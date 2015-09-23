// balst_stacktraceresolverimpl_elf.cpp                               -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <balst_stacktraceresolverimpl_elf.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(balst_stacktraceresolverimpl_elf_cpp,"$Id$ $CSID$")

#include <balst_objectfileformat.h>

#ifdef BALST_OBJECTFILEFORMAT_RESOLVER_ELF

#include <balst_stacktraceresolver_filehelper.h>

#include <bdlb_string.h>

#include <bsls_assert.h>
#include <bsls_platform.h>

#include <bsl_algorithm.h>
#include <bsl_cstddef.h>
#include <bsl_cstdlib.h>
#include <bsl_cerrno.h>
#include <bsl_cstring.h>
#include <bsl_climits.h>
#include <bsl_cstdarg.h>
#include <bsl_vector.h>

#include <elf.h>
#include <unistd.h>

#undef BALST_DWARF
#if defined(BALST_OBJECTFILEFORMAT_RESOLVER_DWARF)
# define BALST_DWARF 1
# include <dwarf.h>
#endif

#if defined(BSLS_PLATFORM_OS_HPUX)

# include <dl.h>
# include <aCC/acxx_demangle.h>

#elif defined(BSLS_PLATFORM_OS_LINUX)

# include <cxxabi.h>
# include <dlfcn.h>
# include <execinfo.h>
# include <link.h>

#elif defined(BSLS_PLATFORM_OS_SOLARIS)

# include <link.h>

# if defined(BSLS_PLATFORM_CMP_GNU) || defined(BSLS_PLATFORM_CMP_CLANG)
#   include <cxxabi.h>
# endif

#else

# error unrecognized ELF platform

#endif

// ============================================================================
//              Debugging trace macros: 'eprintf' and 'zprintf'
// ============================================================================

#undef  TRACES
#define TRACES 0    // 0 == debugging traces off
                    // 1 == debugging traces on
                    // 2 == debugging traces on, eprintf core dumps

#undef eprintf
#undef zprintf
#undef ASSERT_WARN
#undef P
#undef PH
#define EPRINTF

#if TRACES > 0
# include <stdio.h>

#define zprintf printf

#if 1 == TRACES
# define eprintf printf
#else
static
void eprintf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    vprintf(format, ap);

    va_end(ap);

    BSLS_ASSERT_OPT(0 && "eprintf called");
}
#endif

static bool warnHandler(const char *expr,
                        const char *functionName,
                        int         line)
{
    zprintf("Warning: assertion (%s) failed at line %d in function %s\n",
            expr, line, functionName);

    return false;
}

#define ASSERT_WARN(expr)    ((expr) || warnHandler(#expr, (fn), __LINE__))

static bool warnPrint(const char *expr, void *value)
{
    zprintf("%s = %p\n", expr, value);

    return false;
}
static bool warnPrint(const char                        *expr,
                      BloombergLP::bsls::Types::UintPtr  value)
{
    zprintf("%s = %lu\n", expr, value);

    return false;
}
static bool warnPrint(const char *expr, int value)
{
    zprintf("%s = %d\n", expr, value);

    return false;
}

static bool warnPrintHex(const char                        *expr,
                         BloombergLP::bsls::Types::UintPtr  value)
{
    zprintf("%s = 0x%lx\n", expr, value);

    return false;
}

#define P(expr)     warnPrint(   #expr, (expr))
#define PH(expr)    warnPrintHex(#expr, (expr))

#else

static inline
void eprintf(const char *, ...)
    // called on debug output - output is very voluminous if this is turned on
{
}

static inline
void zprintf(const char *, ...)
    // called on debug output - output is very voluminous if this is turned on
{
}


#define ASSERT_WARN(expr)
#define P(expr)
#define PH(expr)

#endif

// Rohan's:
// IMPLEMENTATION NOTES:
//
// All of the following 'struct' definitions are specific to Sun Solaris and
// are derived from '/usr/include/sys/elf.h'.  Note that we use 32-bit
// 'struct's for explanation.
//
// Each ELF object file or executable starts with an ELF header that specifies
// how many segments are provided in the file.  The ELF header looks as
// follows:
//..
// typedef struct {
//     unsigned char e_ident[EI_NIDENT];    //  ident bytes
//     Elf32_Half    e_type;                //  file type
//     Elf32_Half    e_machine;             //  target machine
//     Elf32_Word    e_version;             //  file version
//     Elf32_Addr    e_entry;               //  start address
//     Elf32_Off     e_phoff;               //  program header file offset
//     Elf32_Off     e_shoff;               //  section header file offset
//     Elf32_Word    e_flags;               //  file flags
//     Elf32_Half    e_ehsize;              //  sizeof ehdr
//     Elf32_Half    e_phentsize;           //  sizeof phdr
//     Elf32_Half    e_phnum;               //  number of program headers
//     Elf32_Half    e_shentsize;           //  sizeof section header
//     Elf32_Half    e_shnum;               //  number of section headers
//     Elf32_Half    e_shstrndx;            //  shdr string index
// } Elf32_Ehdr;
//..
// Each segment is described by a program header that is an array of
// structures, each describing a segment or other information the system needs
// to prepare the program for execution, and typically looks as follows:
//..
// typedef struct {
//     Elf32_Word p_type;                   //  entry type
//     Elf32_Off  p_offset;                 //  file offset
//     Elf32_Addr p_vaddr;                  //  virtual address
//     Elf32_Addr p_paddr;                  //  physical address
//     Elf32_Word p_filesz;                 //  file size
//     Elf32_Word p_memsz;                  //  memory size
//     Elf32_Word p_flags;                  //  entry flags
//     Elf32_Word p_align;                  //  memory/file alignment
// } Elf32_Phdr;
//..
// An object file segment contains one or more sections.  The string table
// provides the names of the various sections corresponding to the integral
// 'sh_name'.  An elf file will typically contain sections such as '.text',
// '.data', '.bss' etc.
//..
// typedef struct {
//     Elf32_Word sh_name;                  //  section name
//     Elf32_Word sh_type;                  //  SHT_...
//     Elf32_Word sh_flags;                 //  SHF_...
//     Elf32_Addr sh_addr;                  //  virtual address
//     Elf32_Off  sh_offset;                //  file offset
//     Elf32_Word sh_size;                  //  section size
//     Elf32_Word sh_link;                  //  misc info
//     Elf32_Word sh_info;                  //  misc info
//     Elf32_Word sh_addralign;             //  memory alignment
//     Elf32_Word sh_entsize;               //  entry size if table
// } Elf32_Shdr;
//
// typedef struct
// {
//     Elf32_Word    st_name;               //  Symbol name (string tbl index)
//     Elf32_Addr    st_value;              //  Symbol value
//     Elf32_Word    st_size;               //  Symbol size
//     unsigned char st_info;               //  Symbol type and binding
//     unsigned char st_other;              //  Symbol visibility
//     Elf32_Section st_shndx;              //  Section index - 16-bit
// } Elf32_Sym;
//..
// Below we explain the strategies to resolve symbols on the various platforms
// that we support.
//
// Solaris:
// ----------------------------------------------------------------------------
//
// The _DYNAMIC symbol references a _dynamic structure that refers to the
// linked symbols:
//..
// typedef struct {
//     Elf32_Sword d_tag;                   //  how to interpret value
//     union {
//         Elf32_Word d_val;
//         Elf32_Addr d_ptr;
//         Elf32_Off  d_off;
//     } d_un;
// } Elf32_Dyn;
//..
// Tag values
//..
// #define    DT_NULL      0                //  last entry in list
// #define    DT_DEBUG    21                //  pointer to r_debug structure
//
// struct r_debug {
//     int            r_version;            //  debugging info version no.
//     Link_map      *r_map;                //  address of link_map
//     unsigned long  r_brk;                //  address of update routine
//     r_state_e      r_state;
//     unsigned long  r_ldbase;             //  base addr of ld.so
//     Link_map      *r_ldsomap;            //  address of ld.so.1's link map
//     rd_event_e     r_rdevent;            //  debug event
//     rd_flags_e     r_flags;              //  misc flags.
// };
//..
// The link_map is a chain of loaded object.
//..
// struct link_map {
//
//     unsigned long  l_addr;               // address at which object is
//                                          // mapped
//     char          *l_name;               //  full name of loaded object
// #ifdef _LP64
//     Elf64_Dyn     *l_ld;                 //  dynamic structure of object
// #else
//     Elf32_Dyn     *l_ld;                 //  dynamic structure of object
// #endif
//     Link_map     *l_next;                //  next link object
//     Link_map     *l_prev;                //  previous link object
//     char         *l_refname;             //  filters reference name
// };
//..
// Linux:
// ----------------------------------------------------------------------------
//..
// int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info,
//                                      size_t               size,
//                                      void                *data),
//                     void *data);
//     // Walk through the list of an application's shared objects and invoke
//     // the specified 'callback' (taking the specified 'info' object of
//     // the specified 'size' and specifying the user supplied 'data') using
//     // the specified 'data' to be passed to 'callback'.
//
// struct dl_phdr_info
// {
//     ElfW(Addr)        dlpi_addr;
//     const char       *dlpi_name;
//     const ElfW(Phdr) *dlpi_phdr;
//     ElfW(Half)        dlpi_phnum;
//
//     // Note: the next two members were introduced after the first
//     // version of this structure was available.  Check the SIZE
//     // argument passed to the dl_iterate_phdr() callback to determine
//     // whether or not they are provided.
//
//     //  Incremented when a new object may have been added.
//     unsigned long long int dlpi_adds;
//
//     //  Incremented when an object may have been removed.
//     unsigned long long int dlpi_subs;
// };
//..
// HPUX:
// ----------------------------------------------------------------------------
//..
// struct shl_descriptor {
//     unsigned long  tstart;                   // start address of the shared
//                                              // library text segment
//
//     unsigned long  tend;                     // end address of the shared
//                                              // library text segment
//
//     unsigned long  dstart;
//     unsigned long  dend;
//     void          *ltptr;
//     shl_t          handle;
//     char           filename[MAXPATHLEN + 1];
//     void          *initializer;
//     unsigned long  ref_count;
//     unsigned long  reserved3;
//     unsigned long  reserved2;
//     unsigned long  reserved1;
//     unsigned long  reserved0;
// };
//
// int shl_get_r(int index, struct shl_descriptor *desc);
//     // Load into the specified 'desc' information about the loaded library
//     // at the specified 'index'.  For libraries loaded implicitly (at
//     // startup time), 'index' is the ordinal number of the library as it
//     // appeared on the command line.  Return 0 on success and a non-zero
//     // value otherwise.  Note that an 'index' value of 0 refers to the main
//     // program itself and -1 refers to the dynamic loader.
//..

// ----------------------------------------------------------------------------
// Bill's:
// IMPLEMENTATION NOTES:
//
// The following 'struct' definitions describing the Elf format are modified
// from those found in the .h files (mostly 'elf.h').  The following
// transformations have been done on the definitions found in the .h file --
// typedefs to fundamental types have been resolved (some that are equivalent
// to 'bsls::Types::UintPtr' have been translated to 'UintPtr') and the names
// given for the structs are the names of typedefs to them in the namespace
// 'local' within this file.  Significantly, data members not used in this
// source file are ommitted.
//
// Each ELF object file or executable starts with an ELF header that specifies
// how many segments are provided in the file.  The ELF header looks as
// follows:
//..
// typedef struct {
//     unsigned char e_ident[EI_NIDENT];    //  ident bytes
//     UintPtr       e_phoff;               //  program header file offset
//     UintPtr       e_shoff;               //  section header file offset
//     short         e_shentsize;           //  sizeof section header
//     short         e_shnum;               //  number of section headers
//     short         e_shstrndx;            //  shdr string index
// } local::ElfHeader;
//..
// Each segment is described by a program header that is an array of
// structures, each describing a segment or other information the system needs
// to prepare the program for execution, and typically looks as follows:
//..
// typedef struct {
//     unsigned int p_type;                   //  entry type
//     UintPtr      p_offset;                 //  file offset
//     unsigned int p_vaddr;                  //  virtual address
//     unsigned int p_memsz;                  //  memory size
// } local::ElfProgramHeader;
//..
// An object file segment contains one or more sections.  The string table
// provides the names of the various sections corresponding to the integral
// 'sh_name'.  An elf file will typically contain sections such as '.text',
// '.data', '.bss' etc.
//..
// typedef struct {
//     unsigned int sh_name;                  //  section name
//     unsigned int sh_type;                  //  SHT_...
//     UintPtr      sh_offset;                //  file offset
//     unsigned int sh_size;                  //  section size
// } local::ElfSectionHeader;
//
// typedef struct
// {
//     unsigned int  st_name;               //  Symbol name (string tbl index)
//     UintPtr       st_value;              //  Symbol value
//     unsigned int  st_size;               //  Symbol size
//     unsigned char st_info;               //  Symbol type and binding
// } local::ElfSymbol;
//..
// ----------------------------------------------------------------------------
// The above definitions describe the data within one file.  However, if the
// executable is dynamically linked, that usually being the case, multiple
// files must be traversed.  Unfortunately, no one strategy for traversing the
// files works for more than one platform -- so for the 3 platform currently
// supported, Solaris, Linux, and HPUX, we have a 3 custom strategies.
//
// Solaris:
// ----------------------------------------------------------------------------
//
// The link_map is a node in a chain, each representing a loaded object.
//..
// struct link_map {
//     unsigned long        l_addr;         // address at which object is
//                                          // mapped
//     char                *l_name;         // full name of loaded object
//     local::ElfDynamic   *l_ld;           // dynamic structure of object
//     Link_map            *l_next;         // next link object
// };
//
// struct r_debug {
//     link_map      *r_map;                //  address of link_map
// };
//
// typedef struct {
//     int             d_tag;                   //  how to interpret value
//     union {
//         // Note other interpretations of this union are no used, so they
//         // are omitted here.
//
//         UintPtr     d_ptr;           // really a pointer of type 'r_debug *'
//     } d_un;
// } local::ElfDynamic;
//
// Tag values
//
// #define    DT_NULL      0                //  last entry in list
// #define    DT_DEBUG    21                //  pointer to 'r_debug' structure
//..
// The '_DYNAMIC' symbol is the address of the beginning of an array of objects
// of type 'local::ElfDynamic', one of which contains a pointer to the
// 'r_debug' object, which contains a pointer to the linked list of 'link_map'
// objects, one of which exists for each executable or shared library.
//
// Linux:
// ----------------------------------------------------------------------------
//..
// int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info,
//                                      size_t               size,
//                                      void                *data),
//     // Walk through the list of an application's shared objects and invoke
//     // the specified 'callback' (taking the specified 'info' object of
//     // the specified 'size' and specifying the user supplied 'data') using
//     // the specified 'data' to be passed to 'callback'.
//
// struct dl_phdr_info
// {
//     UintPtr                           dlpi_addr;    // base address
//     const char                       *dlpi_name;    // lib name
//     const local::ElfProgramHeader    *dlpi_phdr;    // array of program
//                                                     // headers
//     short                             dlpi_phnum;   // base address
// };
//..
// HPUX:
// ----------------------------------------------------------------------------
//..
// struct shl_descriptor {
//     unsigned long  tstart;                   // start address of the shared
//                                              // library text segment
//
//     unsigned long  tend;                     // end address of the shared
//                                              // library text segment
//
//     char           filename[MAXPATHLEN + 1];
// };
//
// int shl_get_r(int index, struct shl_descriptor *desc);
//     // Load into the specified 'desc' information about the loaded library
//     // at the specified 'index'.  For libraries loaded implicitly (at
//     // startup time), 'index' is the ordinal number of the library as it
//     // appeared on the command line.  Return 0 on success and a non-zero
//     // value otherwise.  Note that an 'index' value of 0 refers to the main
//     // program itself and -1 refers to the dynamic loader.
//..
// DWARF:
// ----------------------------------------------------------------------------
//..
// The DWARF information is in 3 sections of the segments:
// .debug_aranges:     Specifies addresses ranges, and for each range, the
//                     offset of the compilation unit for addresss within those
//                     ranges in the .debug_info section.
// .debug_info:        Various information about the compilation, including the
//                     source file directory and name and the offset in the
//                     .debug_line section of the line number information.
// .debug_line:        Line number information.
//..

namespace BloombergLP {

namespace {

namespace local {

typedef bsls::Types::UintPtr UintPtr;
typedef bsls::Types::IntPtr  IntPtr;

typedef balst::StackTraceResolverImpl<balst::ObjectFileFormat::Elf>
                                                            StackTraceResolver;

                            // --------------------------
                            // Run-Time Platform Switches
                            // --------------------------

#if defined(BSLS_PLATFORM_OS_LINUX)
enum { e_IS_LINUX = 1 };
#else
enum { e_IS_LINUX = 0 };
#endif

#if   defined(BSLS_PLATFORM_IS_BIG_ENDIAN)
enum { e_IS_BIG_ENDIAN = 1,
       e_IS_LITTLE_ENDIAN = 0 };
#elif defined(BSLS_PLATFORM_IS_LITTLE_ENDIAN)
enum { e_IS_BIG_ENDIAN = 0,
       e_IS_LITTLE_ENDIAN = 1 };
#else
# error endianness is undefined
#endif

                                   // ---------
                                   // Constants
                                   // ---------

enum {
    k_SCRATCH_BUF_LEN = 32 * 1024 - 64,    // length in bytes of
                                           // d_scratchBuf_p, 32K minus a
                                           // little so we don't waste a page

    k_SYMBOL_BUF_LEN  = k_SCRATCH_BUF_LEN  // length in bytes of d_symbolBuf_p
};

                                // ---------------
                                // local Elf Types
                                // ---------------

#undef   SPLICE
#ifdef BSLS_PLATFORM_CPU_64_BIT
# define SPLICE(suffix)  Elf64_ ## suffix
#else
# define SPLICE(suffix)  Elf32_ ## suffix
#endif

// The following types are part of the ELF standard, and describe structs that
// occur in the executable file / shared libraries.

typedef SPLICE(Dyn)     ElfDynamic;        // The expression '&_DYNAMIC' is a
                                           // 'void *' pointer to a an array of
                                           // 'struct's of this type, used to
                                           // find the link map on Solaris.

typedef SPLICE(Ehdr)    ElfHeader;         // The elf header is a standard
                                           // header at the start of any ELF
                                           // file

typedef SPLICE(Phdr)    ElfProgramHeader;  // Program headers are obtained from
                                           // the link map.  We use them to
                                           // find code segments.

typedef SPLICE(Shdr)    ElfSectionHeader;  // Section headers are located from
                                           // the U::ElfHeader, they tell us
                                           // where the sections containing
                                           // symbols and strings are

typedef SPLICE(Sym)     ElfSymbol;         // Describes one symbol in the
                                           // symbol table.
#undef SPLICE

struct Section {
    // Refers to one section of a segment.

    // DATA
    UintPtr d_offset;    // offset of the section in the file
    UintPtr d_size;      // size of that section

    // CREATOR
    Section()
    : d_offset(0)
    , d_size(0)
    {
    }

    // MANIPULATOR
    void reset(UintPtr offset = 0, UintPtr size = 0)
    {
        d_offset = offset;
        d_size   = size;
    }

    // ACCESSORS
    bool contains(UintPtr offset) const
    {
        return d_offset <= offset && offset <= d_offset + d_size;
    }

    bool contains(const void *address) const
    {
        return contains(reinterpret_cast<UintPtr>(address));
    }

    bool contains(const Section& section) const
    {
        return d_offset <= section.d_offset &&
                        section.d_offset + section.d_size <= d_offset + d_size;
    }

    bool overlaps(const Section& section) const
    {
        return d_offset <= section.d_offset
               ? d_offset + d_size > section.d_offset
               : section.d_offset + section.d_size > d_offset;
    }
};

#ifdef BALST_DWARF
template <class TYPE>
inline
void readValue(TYPE        *dst,
               const char **readPtr)
    // Copy the memory pointed at by the specified '*readPtr' to the specified
    // '*dst', then increment the '*readPtr' by the size of the memory read.
{
    bsl::memcpy(dst, *readPtr, sizeof(*dst));
    *readPtr += sizeof(*dst);
}

inline
UintPtr minArangesReadLength(bool shortInitialLength,
                             bool shortAddresses)
{
    enum { MIN_SIZE_INITIAL_LENGTH    = sizeof(unsigned),
           MAX_SIZE_INITIAL_LENGTH    = sizeof(unsigned) + sizeof(UintPtr),
           SIZE_VERSION               = sizeof(short),
           MIN_SIZE_DEBUG_INFO_OFFSET = sizeof(unsigned),
           MAX_SIZE_DEBUG_INFO_OFFSET = sizeof(UintPtr),
           SIZE_ADDRESS_SIZE          = sizeof(char),
           SIZE_SEGMENT_SIZE          = sizeof(char),
           MIN_SIZE_PAIR_PAIRS        = 4 * sizeof(unsigned),
           MAX_SIZE_PAIR_PAIRS        = 4 * sizeof(UintPtr) };

#ifdef BSLS_PLATFORM_CPU_64_BIT
   return SIZE_VERSION + SIZE_ADDRESS_SIZE + SIZE_SEGMENT_SIZE +
                     (shortInitialLength
                      ? MIN_SIZE_INITIAL_LENGTH + MIN_SIZE_DEBUG_INFO_OFFSET
                      : MAX_SIZE_INITIAL_LENGTH + MAX_SIZE_DEBUG_INFO_OFFSET) +
                                        (shortAddresses ? MIN_SIZE_PAIR_PAIRS
                                                        : MAX_SIZE_PAIR_PAIRS);
#else
   return SIZE_VERSION + SIZE_ADDRESS_SIZE + SIZE_SEGMENT_SIZE +
                         MIN_SIZE_INITIAL_LENGTH + MIN_SIZE_DEBUG_INFO_OFFSET +
                                                           MIN_SIZE_PAIR_PAIRS;
#endif
}
#endif

                                    // --------
                                    // FrameRec
                                    // --------

class FrameRec {
    // A struct consisting of the things we want stored associated with a given
    // frame.  We put these into a vector and sort them for O(log n) lookup by
    // address.

    // DATA
    const void             *d_address;
    balst::StackTraceFrame *d_frame_p;
#ifdef BALST_DWARF
    UintPtr                 d_compileUnitOffset;
    local::Section          d_lineNumbers;
#endif
    bool                    d_isDone;        // This has differrent meanings in
                                             // different contexts.  When
                                             // resolving, this flag, if true,
                                             // means the symbol in this frame
                                             // is resolved.  When reading
                                             // DWARF information, means this
                                             // frame has had line numbers and
                                             // source file name resolved.

  public:
    // CREATORS
    explicit
    FrameRec(const void             *address,
             balst::StackTraceFrame *framePtr = 0)
    : d_address(address)
    , d_frame_p(framePtr)
#ifdef BALST_DWARF
    , d_compileUnitOffset(0)
    , d_lineNumbers()
#endif
    , d_isDone(false)
        // Create a 'FrameRec' referring to the specified 'address' and the
        // specified 'framePtr'.
    {
    }

    // FrameRec(const FrameRec&) = default;

    // ~FrameRec() = default;

    // MANIPULATORS
    // inline operator=(const FrameRec&) = default;

    void setAddress(const void *value)
    {
        d_address = value;
    }

    void setAddress(const UintPtr value)
    {
        d_address = reinterpret_cast<const void *>(value);
    }

#ifdef BALST_DWARF
    void setCompileUnitOffset(const UintPtr value)
    {
        d_compileUnitOffset = value;
    }
#endif

    void setDone()
        // Set this frame as being done.
    {
        d_isDone = true;
    }

    void setNotDone()
        // Set this frame as being done.
    {
        d_isDone = false;
    }

    // ACCESSORS
    bool operator<(const FrameRec& rhs) const
        // Return 'true' if the address field of this object is less than the
        // address field of 'rhs'.
    {
        return d_address < rhs.d_address;
    }

    const void *address() const
        // Return the 'address' field from this object.
    {
        return d_address;
    }

#ifdef BALST_DWARF
    UintPtr compileUnitOffset()
    {
        return d_compileUnitOffset;
    }
#endif

    balst::StackTraceFrame& frame() const
        // Return a reference to the modifiable 'frame' referenced by this
        // object.  Note that though this is a 'const' method, modifiable
        // access to the frame is provided.
    {
        return *d_frame_p;
    }

    bool isDone() const
        // Return 'true' if this frame is done and 'false' otherwise.
    {
        return d_isDone;
    }
};

typedef bsl::vector<local::FrameRec> FrameRecVec;     // Vector of 'FrameRec's.
typedef FrameRecVec::iterator        FrameRecVecIt;   // Iterator of
                                                      // 'FrameRecVec'.

}  // close namespace local

typedef bsl::size_t size_t;

}  // close unnamed namespace

                              // ----------------
                              // static functions
                              // ----------------

static
int checkElfHeader(local::ElfHeader *elfHeader)
    // Return 0 if the magic numbers in the specified 'elfHeader' are correct
    // and a non-zero value otherwise.
{
    if   (ELFMAG0 != elfHeader->e_ident[EI_MAG0]
       || ELFMAG1 != elfHeader->e_ident[EI_MAG1]
       || ELFMAG2 != elfHeader->e_ident[EI_MAG2]
       || ELFMAG3 != elfHeader->e_ident[EI_MAG3]) {
        return -1;                                                    // RETURN
    }

    // this code can only read native-endian ELF files

    if ((local::e_IS_BIG_ENDIAN ? ELFDATA2MSB : ELFDATA2LSB) !=
                                                 elfHeader->e_ident[EI_DATA]) {
        return -1;                                                    // RETURN
    }

    // this code can only read native-sized ELF files

    if ((sizeof(void *) == 4 ? ELFCLASS32 : ELFCLASS64) !=
                                                elfHeader->e_ident[EI_CLASS]) {
        return -1;                                                    // RETURN
    }

    return 0;
}

                  // -----------------------------------------
                  // local::StackTraceResolver::CurrentSegment
                  // -----------------------------------------

struct local::StackTraceResolver::CurrentSegment {
    // This 'struct' contains all fields of this resolver that are local to the
    // current segment.  The resolver iterates over multiple segments,
    // resolving symbols within one at a time.

    // TYPES
    typedef local::UintPtr
                   UintPtr;             // 32 bit unsigned on 32 bit, 64 bit
                                        // unsigned on 64 bit, usually used for
                                        // absolute offsets into a file

    // DATA
    StackTraceResolver_FileHelper
                  *d_helper_p;          // file helper associated with current
                                        // segment

    StackTrace    *d_stackTrace_p;      // the stack trace we are resolving

    local::FrameRecVec
                   d_frameRecs;         // Vector of address frame pairs for
                                        // fast lookup of addresses.  Not local
                                        // to current segment -- initialized
                                        // once per resolve.

    local::FrameRecVecIt                // This begin, end pair indicates the
                   d_frameRecsBegin;    // range of frame records that pertain
    local::FrameRecVecIt                // to the current segment.
                   d_frameRecsEnd;

    UintPtr        d_adjustment;        // adjustment between addresses
                                        // expressed in object file and actual
                                        // addresses in memory for current
                                        // segment

    local::Section d_symTableSec;       // symbol table section (symbol table
                                        // does not contain symbol names, just
                                        // offsets into string table) from the
                                        // beginning of the executable or
                                        // library file

    local::Section d_stringTableSec;    // string table offset from the
                                        // beginning of the file

#ifdef BALST_DWARF
    local::Section d_arangesSec;        // .debug_aranges section

    local::Section d_infoSec;           // .debug_info section

    local::Section d_lineSec;           // .debug_line section
#endif

    char          *d_scratchBuf_p;      // scratch buffer (from resolver)

    char          *d_symbolBuf_p;       // scratch space for symbols (from
                                        // resolver)

    int            d_numTotalUnmatched; // Total number of unmatched frames
                                        // remaining in this resolve.

    bool           d_isMainExecutable;  // 'true' if in main executable
                                        // segment, as opposed to a shared
                                        // library

  private:
    // NOT IMPLEMENTED
    CurrentSegment(const CurrentSegment&);
    CurrentSegment& operator=(const CurrentSegment&);

  public:
    // CREATORS
    CurrentSegment(local::StackTraceResolver *resolver);
        // Create this 'Seg' object, initialize 'd_numFrames' to 'numFrames',
        // and initialize all other fields to 0.

    // MANIPULATORS
#ifdef BALST_DWARF
    int readAranges();
        // Read the .debug_aranges section.

    int readDwarf();
        // Read the DWARF information.
#endif

    void reset();
        // Zero numerous fields.
};

// CREATORS
local::StackTraceResolver::CurrentSegment::CurrentSegment(
                                           local::StackTraceResolver *resolver)
: d_helper_p(0)
, d_stackTrace_p(resolver->d_stackTrace_p)
, d_frameRecs(&resolver->d_hbpAlloc)
, d_frameRecsBegin()
, d_frameRecsEnd()
, d_adjustment(0)
, d_symTableSec()
, d_stringTableSec()
#ifdef BALST_DWARF
, d_arangesSec()
, d_infoSec()
, d_lineSec()
#endif
, d_scratchBuf_p(resolver->d_scratchBuf_p)
, d_symbolBuf_p( resolver->d_symbolBuf_p)
, d_numTotalUnmatched(resolver->d_stackTrace_p->length())
, d_isMainExecutable(0)
{
    d_frameRecs.reserve(d_numTotalUnmatched);
    for (int ii = 0; ii < d_numTotalUnmatched; ++ii) {
        balst::StackTraceFrame& frame = (*resolver->d_stackTrace_p)[ii];
        d_frameRecs.push_back(local::FrameRec(frame.address(), &frame));
    }
    bsl::sort(d_frameRecs.begin(), d_frameRecs.end());
}

// MANIPULATORS
#ifdef BALST_DWARF
int local::StackTraceResolver::CurrentSegment::readAranges()
{
    static const char fn[] = { "readAranges:" };

    local::FrameRecVecIt end = d_frameRecsEnd;
    int matched = static_cast<int>(end - d_frameRecsBegin);
    BSLS_ASSERT(matched > 0);    // otherwise we should not have been called.

    zprintf("%s starting, matched=%d\n", fn, matched);

    UintPtr offset     = d_arangesSec.d_offset;
    UintPtr arangesEnd = offset + d_arangesSec.d_size;

    local::Section  addressRange, prevRange;
    local::FrameRec dummyFrameRec(0);

    const int shortMinReadLength = local::minArangesReadLength(true, true);

    const char *readPtr;
    for (; offset < arangesEnd; offset += readPtr - d_scratchBuf_p) {
        int lengthToRead = static_cast<int>(
                                    bsl::min<UintPtr>(local::k_SCRATCH_BUF_LEN,
                                                      arangesEnd - offset));
        if (lengthToRead < shortMinReadLength) {
            eprintf("%s Unacceptably short read %u bytes, < min %d bytes\n",
                                         fn, lengthToRead, shortMinReadLength);
            return -1;                                                // RETURN
        }

        int rc = d_helper_p->readExact(d_scratchBuf_p,
                                       lengthToRead,
                                       offset);
        if (rc) {
            eprintf("fn read failed\n", fn);
            return -1;                                                // RETURN
        }

        // Note 'arangesEndPtr' may point way past the end of the buffer.

        const char *arangesEndPtr = d_scratchBuf_p +
                        d_arangesSec.d_size - (offset - d_arangesSec.d_offset);

        readPtr = d_scratchBuf_p;
        const char *endPtr = readPtr + lengthToRead;
        BSLS_ASSERT(endPtr <= arangesEndPtr);
        for (;;) {
            if (endPtr - readPtr < local::minArangesReadLength(true, true)) {
                break;    // go back and re-read buffer
            }

            const char *startPtr = readPtr;

            UintPtr rangeLength;
            unsigned u, u2;
            local::readValue(&u, &readPtr);
            bool isShortInitialLength;
            if (0xffffffff == u) {
                local::readValue(&rangeLength, &readPtr);
                isShortInitialLength = false;
            }
            else {
                if (u >= 0xfffffff0) {
                    eprintf("%s illegal preNum:0x%x\n", fn, u);
                    return -1;                                        // RETURN
                }
                isShortInitialLength = true;
                rangeLength = u;
            }
            const bool isLongRangeLength =
                                   rangeLength > local::k_SCRATCH_BUF_LEN - 12;
            if (isLongRangeLength) {
                zprintf("Long range, length %lu, encountered\n", rangeLength);
            }
            const char *rangeEnd = readPtr + rangeLength;
            if (rangeEnd > arangesEndPtr) {
                eprintf("%s attempt to read 0%ld bytes past end of aranges\n",
                                                 fn, rangeEnd - arangesEndPtr);
                return -1;
            }
            if (rangeEnd > endPtr) {
                if (rangeEnd > arangesEndPtr) {
                    eprintf("%s range end:%p > arangesEndPtr:%p\n", fn,
                                                      rangeEnd, arangesEndPtr);
                    return -1;                                        // RETURN
                }

                if (!isLongRangeLength) {
                    readPtr = startPtr;
                    break;    // go back and re-read buffer
                }
            }
            BSLS_ASSERT(rangeEnd <= endPtr || isLongRangeLength);
            unsigned short version;
            local::readValue(&version, &readPtr);
            if (2 != version) {
                eprintf("%s invalid version:%u\n", fn, version);
                return -1;                                            // RETURN
            }
            UintPtr debugInfoOffset;
            if (isShortInitialLength) {
                local::readValue(&u, &readPtr);
                debugInfoOffset = u;
            }
            else {
                local::readValue(&debugInfoOffset, &readPtr);
            }
            if (!debugInfoOffset > d_infoSec.d_size) {
                eprintf("%s invalid debugInfoOffset:%lx\n",
                                                          fn, debugInfoOffset);
                return -1;                                            // RETURN
            }
            zprintf("%s %s debugInfoOffset: 0x%lx\n", fn,
                     isShortInitialLength ? "short" : "long", debugInfoOffset);
            const unsigned char addressSize = *readPtr++;
#ifdef BSLS_PLATFORM_CPU_64_BIT
            bool isShortAddressSize;
            if      (sizeof(UintPtr)  == addressSize) {
                isShortAddressSize = false;
            }
            else if (sizeof(unsigned) == addressSize) {
                isShortAddressSize = true;
            }
            else {
#else
            const bool isShortAddressSize = false;
            if (sizeof(UintPtr) != addressSize) {
#endif
                eprintf("%s invalid address size:%u\n", fn, addressSize);
                return -1;                                            // RETURN
            }
            // zprintf("%s addressSize: %u\n", fn, addressSize);

            if (endPtr - startPtr < local::minArangesReadLength(
                                                         isShortInitialLength,
                                                         isShortAddressSize)) {
                readPtr = startPtr;
                break;    // go back and re-read buffer
            }
            const unsigned char segmentSize = *readPtr++;
            if (0 != segmentSize) {
                eprintf("%s invalid segment size:%u\n", fn, segmentSize);
                return -1;                                            // RETURN
            }
            readPtr += sizeof(unsigned);    // This isn't in the spec, but
                                            // everything didn't start working
                                            // until I put it in.

            if (0 != (rangeEnd - readPtr) % (isShortAddressSize
                                             ? 2 * sizeof(unsigned)
                                             : 2 * sizeof(UintPtr))) {
                eprintf("%s strange rangeLength\n", fn);
                return -1;                                            // RETURN
            }
            for (;;) {
                const char *afterReadPtr = readPtr + 2 * addressSize;

                if (afterReadPtr > rangeEnd) {
                    eprintf("%s set not terminated by 0's\n", fn);
                    return -1;                                        // RETURN
                }
                if (afterReadPtr > endPtr) {
                    BSLS_ASSERT(isLongRangeLength);

                    // We're in a long range and have run to the end of the
                    // buffer.  Read more.

                    const unsigned shift = readPtr - d_scratchBuf_p;
                    rangeEnd      -= shift;
                    offset        += shift;
                    lengthToRead = static_cast<int>(
                                    bsl::min<UintPtr>(local::k_SCRATCH_BUF_LEN,
                                                      arangesEnd - offset));
                    rc = d_helper_p->readExact(d_scratchBuf_p,
                                               lengthToRead,
                                               offset);
                    if (rc) {
                        eprintf("fn read failed\n", fn);

                        return -1;                                    // RETURN
                    }
                    readPtr       = d_scratchBuf_p;
                    startPtr      = d_scratchBuf_p;
                    endPtr        = d_scratchBuf_p + lengthToRead;
                    arangesEndPtr = d_scratchBuf_p + (arangesEnd - offset);
                }

#ifdef BSLS_PLATFORM_CPU_64_BIT
                if (isShortAddressSize) {
                    local::readValue(&u,  &readPtr);
                    local::readValue(&u2, &readPtr);
                    addressRange.reset(u, u2);
                }
                else {
                    local::readValue(&addressRange.d_offset, &readPtr);
                    local::readValue(&addressRange.d_size,   &readPtr);
                }
#else
                local::readValue(&addressRange.d_offset, &readPtr);
                local::readValue(&addressRange.d_size,   &readPtr);
#endif

                if (0 == addressRange.d_offset && 0 == addressRange.d_size) {
                    if (readPtr != rangeEnd) {
                        eprintf("%s terminating 0's %s range end\n", fn,
                                          readPtr < rangeEnd ? "reached before"
                                                             : "overlap");
                        readPtr = rangeEnd;
                    }

                    break;
                }

#ifdef BSLS_ASSERT_LEVEL_ASSERT_SAFE
                if (prevRange.overlaps(addressRange)) {
                    eprintf("%s overlapping ranges [%p, %p) & [%p, %p)\n",
                                          prevRange.d_offset, prevRange.d_size,
                                   addressRange.d_offset, addressRange.d_size);
                    return -1;                                        // RETURN
                }
                prevRange = addressRange;
#endif

                // zprintf("%s range:[%p, 0x%lx)\n", fn, addressRange.d_offset,
                //                                        addressRange.d_size);

                dummyFrameRec.setAddress(addressRange.d_offset);
                local::FrameRecVecIt begin =
                        bsl::lower_bound(d_frameRecsBegin, end, dummyFrameRec);
                for (local::FrameRecVecIt it = begin; it < end &&
                                  addressRange.contains(it->address()); ++it) {
                    const bool isRedundant = 0 != it->compileUnitOffset();
                    zprintf("%s%s range (%p, 0x%lx) matches frame %ld,"
                                                    " address: %p\n",
                                           fn, isRedundant ? " redundant" : "",
                                    addressRange.d_offset, addressRange.d_size,
                                          &it->frame() - &(*d_stackTrace_p)[0],
                                                                it->address());
                    if (isRedundant) {
                        continue;
                    }

                    it->setCompileUnitOffset(d_infoSec.d_offset +
                                                              debugInfoOffset);
                    if (0 == --matched) {
                        zprintf("%s last frame matched\n", fn);
                        return 0;                                     // RETURN
                    }
                }
            }
        }
    }

    zprintf("%s failed to complete -- %d frames unmatched.\n", fn, matched);

    return 0;
}

int local::StackTraceResolver::CurrentSegment::readDwarf()
{
    if (0 == d_arangesSec.d_offset || 0 == d_infoSec.d_offset) {
        zprintf("readDwarf: Not enough information to find file names or line"
                                                                " numbers.\n");
        return -1;
    }

    int rc = readAranges();      // Get the locations of the compile unit info
    if (rc) {                    // for each frame, from .debug_aranges.
        zprintf("readDwarf: .debug_aranges failed\n");
        return -1;
    }
#if 0
    rc = readInfo();             // Get the source filenames and the location
    if (rc) {                    // of the line number info, from .debug_info.
        zprintf("readDwarf: .debug_info failed\n");
        return -1;
    }

    if (0 == d_lineSec.d_offset) {
        zprintf("readDwarf: Line number info not found.\n");
        return -1;
    }

    rc = readLine();             // Get the line numbers, from .debug_line.
    if (rc) {
        zprintf("readDwarf: .debug_line failed\n");
        return -1;
    }
#endif
    return 0;
}
#endif

void local::StackTraceResolver::CurrentSegment::reset()
{
    // Note that 'd_frameRecs' and 'd_numTotalUnmatched' are not to be cleared
    // or reinitialized, they have a lifetime of the length of the resolve.

    d_helper_p           = 0;
    d_frameRecsBegin     = local::FrameRecVecIt();
    d_frameRecsEnd       = local::FrameRecVecIt();
    d_adjustment         = 0;
    d_symTableSec.reset();
    d_stringTableSec.reset();
#ifdef BALST_DWARF
    d_arangesSec.reset();
    d_infoSec.reset();
    d_lineSec.reset();
#endif
}

     // -----------------------------------------------------------------
     // class balst::StackTraceResolverImpl<balst::ObjectFileFormat::Elf>
     //                 == class U::StackTraceResolver
     // -----------------------------------------------------------------

// PRIVATE CREATORS
local::StackTraceResolver::StackTraceResolverImpl(
                                    balst::StackTrace *stackTrace,
                                    bool               demanglingPreferredFlag)
: d_stackTrace_p(stackTrace)
, d_scratchBuf_p(0)
, d_symbolBuf_p(0)
, d_demangle(demanglingPreferredFlag)
, d_hbpAlloc()
{
    d_scratchBuf_p = static_cast<char *>(
                                d_hbpAlloc.allocate(local::k_SCRATCH_BUF_LEN));
    d_symbolBuf_p  = static_cast<char *>(
                                 d_hbpAlloc.allocate(local::k_SYMBOL_BUF_LEN));
    d_seg_p        = new (d_hbpAlloc) CurrentSegment(this);
}

local::StackTraceResolver::~StackTraceResolverImpl()
{
}

// PRIVATE MANIPULATORS
int local::StackTraceResolver::resolveSegment(void       *segmentBaseAddress,
                                              void       *segmentPtr,
                                              UintPtr     segmentSize,
                                              const char *libraryFileName)
{
    int rc;

    const char *sp = static_cast<char *>(segmentPtr);
    const char *se = sp + segmentSize;

    d_seg_p->d_frameRecsBegin = bsl::lower_bound(d_seg_p->d_frameRecs.begin(),
                                                 d_seg_p->d_frameRecs.end(),
                                                 local::FrameRec(sp));
    d_seg_p->d_frameRecsEnd   = bsl::lower_bound(d_seg_p->d_frameRecs.begin(),
                                                 d_seg_p->d_frameRecs.end(),
                                                 local::FrameRec(se));

    int matched = static_cast<int>(
                          d_seg_p->d_frameRecsEnd - d_seg_p->d_frameRecsBegin);
    BSLS_ASSERT(0 <= matched);
    BSLS_ASSERT(matched <= d_stackTrace_p->length());

    zprintf("ResolveSegment lfn=%s\nba=%p sp=%p se=%p matched=%d\n",
            libraryFileName, segmentBaseAddress, sp, se, matched);

    if (0 == matched) {
        zprintf("0 addresses match in library %s\n", libraryFileName);

        return 0;                                                     // RETURN
    }

    local::FrameRecVecIt it, end  = d_seg_p->d_frameRecsEnd;
    for (it = d_seg_p->d_frameRecsBegin; it < end; ++it) {
        zprintf("address %p MATCH\n", it->address());
        it->frame().setLibraryFileName(libraryFileName);
    }

    // read the elf header

    local::ElfHeader elfHeader;
    rc = d_seg_p->d_helper_p->readExact(&elfHeader,
                                        sizeof(local::ElfHeader),
                                        0);
    if (0 != rc) {
        return -1;                                                    // RETURN
    }

    if (0 != checkElfHeader(&elfHeader)) {
        return -1;                                                    // RETURN
    }

    d_seg_p->d_adjustment = reinterpret_cast<UintPtr>(segmentBaseAddress);

    // find the section headers we're interested in, that is, .symtab and
    // .strtab, or, if the file was stripped, .dynsym and .dynstr

    local::ElfSectionHeader symTabHdr, strTabHdr, dynSymHdr, dynStrHdr;
    bsl::memset(&symTabHdr, 0, sizeof(local::ElfSectionHeader));
    bsl::memset(&strTabHdr, 0, sizeof(local::ElfSectionHeader));
    bsl::memset(&dynSymHdr, 0, sizeof(local::ElfSectionHeader));
    bsl::memset(&dynStrHdr, 0, sizeof(local::ElfSectionHeader));

    // Possible speedup: read all the section headers at once instead of one at
    // a time.

    int     numSections = elfHeader.e_shnum;
    UintPtr sectionHeaderSize = elfHeader.e_shentsize;
    UintPtr sectionHeaderOffset = elfHeader.e_shoff;
    if (local::k_SCRATCH_BUF_LEN < sectionHeaderSize) {
        return -1;                                                    // RETURN
    }
    local::ElfSectionHeader *sec = static_cast<local::ElfSectionHeader *>(
                                          static_cast<void *>(d_scratchBuf_p));

    // read the string table that is used for section names

    int     stringSectionIndex = elfHeader.e_shstrndx;
    UintPtr stringSectionHeaderOffset =
                  sectionHeaderOffset + stringSectionIndex * sectionHeaderSize;
    if (0 != d_seg_p->d_helper_p->readExact(sec,
                                            sectionHeaderSize,
                                            stringSectionHeaderOffset)) {
        return -1;                                                    // RETURN
    }
    UintPtr headerStringsOffset = sec->sh_offset;

    for (int i = 0; i < numSections; ++i) {
        if (0 != d_seg_p->d_helper_p->readExact(sec,
                                                sectionHeaderSize,
                                                sectionHeaderOffset +
                                                i * sectionHeaderSize)) {
            return -1;                                                // RETURN
        }
        char sectionName[16];
        if (0 !=  d_seg_p->d_helper_p->readExact(sectionName,
                                                 sizeof(sectionName),
                                                 headerStringsOffset +
                                                               sec->sh_name)) {
            return -1;                                                // RETURN
        }

        // zprintf("Other section: type:%d name:%s\n", sec->sh_type,
        //                                                        sectionName);

        switch (sec->sh_type) {
          case SHT_STRTAB: {
            if      (!bsl::strcmp(sectionName, ".strtab")) {
                strTabHdr = *sec;
            }
            else if (!bsl::strcmp(sectionName, ".dynstr")) {
                dynStrHdr = *sec;
            }
          } break;
          case SHT_SYMTAB: {
            if      (!bsl::strcmp(sectionName, ".symtab")) {
                symTabHdr = *sec;
            }
          } break;
          case SHT_DYNSYM: {
            if      (!bsl::strcmp(sectionName, ".dynsym")) {
                dynSymHdr = *sec;
            }
          } break;
#ifdef BALST_DWARF
          case SHT_PROGBITS: {
            if ('d' != sectionName[1]) {
                ; // do nothing
            }
            else if (!bsl::strcmp(sectionName, ".debug_aranges")) {
                d_seg_p->d_arangesSec.reset(sec->sh_offset, sec->sh_size);
            }
            else if (!bsl::strcmp(sectionName, ".debug_info")) {
                d_seg_p->d_infoSec.   reset(sec->sh_offset, sec->sh_size);
            }
            else if (!bsl::strcmp(sectionName, ".debug_line")) {
                d_seg_p->d_lineSec.   reset(sec->sh_offset, sec->sh_size);
            }
          } break;
#endif
        }
    }

    zprintf("symtab:(0x%lx, %lu), strtab:(0x%lx, %lu)\n",
            symTabHdr.sh_offset, symTabHdr.sh_size, strTabHdr.sh_offset,
            strTabHdr.sh_size);
    zprintf("dynsym:(0x%lx, %lu), dynstr:(0x%lx, %lu)\n",
            dynSymHdr.sh_offset, dynSymHdr.sh_size, dynStrHdr.sh_offset,
            dynStrHdr.sh_size);

    if (0 != strTabHdr.sh_size && 0 != symTabHdr.sh_size) {
        // use the full symbol table if it is available

        d_seg_p->d_symTableSec.   reset(symTabHdr.sh_offset,
                                        symTabHdr.sh_size);
        d_seg_p->d_stringTableSec.reset(strTabHdr.sh_offset,
                                        strTabHdr.sh_size);
    }
    else if (0 != dynSymHdr.sh_size && 0 != dynStrHdr.sh_size) {
        // otherwise use the dynamic symbol table

        d_seg_p->d_symTableSec.   reset(dynSymHdr.sh_offset,
                                        dynSymHdr.sh_size);
        d_seg_p->d_stringTableSec.reset(dynStrHdr.sh_offset,
                                        dynStrHdr.sh_size);
    }
    else {
        // otherwise fail

        return -1;                                                    // RETURN
    }

    zprintf("Sym table:(0x%lx, %lu) string table:(0x%lx %lu)\n",
            d_seg_p->d_symTableSec.d_offset,
            d_seg_p->d_symTableSec.d_size,
            d_seg_p->d_stringTableSec.d_offset,
            d_seg_p->d_stringTableSec.d_size);

#ifdef BALST_DWARF
    zprintf("Aranges:(0x%lx, %lu) info:(0x%lx %lu) line::(0x%lx %lu)\n",
            d_seg_p->d_arangesSec.d_offset,
            d_seg_p->d_arangesSec.d_size,
            d_seg_p->d_infoSec.d_offset,
            d_seg_p->d_infoSec.d_size,
            d_seg_p->d_lineSec.d_offset,
            d_seg_p->d_lineSec.d_size);
#endif

    rc = loadSymbols(matched);
    if (rc) {
        eprintf("loadSymbols failed\n");
        return -1;                                                    // RETURN
    }

    // we return 'rc' at the end.

#ifdef BALST_DWARF
    rc = d_seg_p->readDwarf();
    if (rc) {
        zprintf("readDwarf failed\n");
        return -1;                                                    // RETURN
    }
#endif

    if (0 == (d_seg_p->d_numTotalUnmatched -= matched)) {
        zprintf("Last address matched\n");
    }

    return 0;
}

int local::StackTraceResolver::loadSymbols(int matched)
{
    const int     symSize = static_cast<int>(sizeof(local::ElfSymbol));
    const UintPtr maxSymbolsPerPass = local::k_SYMBOL_BUF_LEN / symSize;
    const UintPtr numSyms = d_seg_p->d_symTableSec.d_size / symSize;
    UintPtr       sourceFileNameOffset = ~static_cast<UintPtr>(0);

    UintPtr      numSymsThisTime;
    for (UintPtr symIndex = 0; symIndex < numSyms;
                                                 symIndex += numSymsThisTime) {
        numSymsThisTime = bsl::min(numSyms - symIndex, maxSymbolsPerPass);

        const UintPtr offsetToRead = d_seg_p->d_symTableSec.d_offset +
                                                            symIndex * symSize;
        int           rc = d_seg_p->d_helper_p->readExact(
                                                     d_symbolBuf_p,
                                                     numSymsThisTime * symSize,
                                                     offsetToRead);
        if (rc) {
            eprintf("failed to read %lu symbols from offset %lu, errno %d\n",
                    numSymsThisTime,
                    offsetToRead,
                    errno);
            return -1;                                                // RETURN
        }

        const local::ElfSymbol *symBufStart = static_cast<local::ElfSymbol *>(
                                           static_cast<void *>(d_symbolBuf_p));
        const local::ElfSymbol *symBufEnd   = symBufStart + numSymsThisTime;
        for (const local::ElfSymbol *sym = symBufStart; sym < symBufEnd;
                                                                       ++sym) {
            switch (ELF32_ST_TYPE(sym->st_info)) {
              case STT_FILE: {
                sourceFileNameOffset = sym->st_name;
              } break;
              case STT_FUNC: {
                if (SHN_UNDEF != sym->st_shndx) {
                    const void *symbolAddress = reinterpret_cast<const void *>(
                                        sym->st_value + d_seg_p->d_adjustment);
                    const void *endSymbolAddress =
                                     static_cast<const char *>(symbolAddress) +
                                                                  sym->st_size;
                    const local::FrameRecVecIt begin =
                              bsl::lower_bound(d_seg_p->d_frameRecsBegin,
                                               d_seg_p->d_frameRecsEnd,
                                               local::FrameRec(symbolAddress));
                    const local::FrameRecVecIt end =
                           bsl::lower_bound(d_seg_p->d_frameRecsBegin,
                                            d_seg_p->d_frameRecsEnd,
                                            local::FrameRec(endSymbolAddress));
                    for (local::FrameRecVecIt it = begin; it < end; ++it) {
                        if (it->isDone()) {
                            continue;
                        }

                        balst::StackTraceFrame& frame = it->frame();

                        frame.setOffsetFromSymbol(
                                  static_cast<const char *>(it->address())
                                   - static_cast<const char *>(symbolAddress));

                        // in ELF, filename information is only accurate
                        // for statics in the main executable

                        if (d_seg_p->d_isMainExecutable
                           && STB_LOCAL == ELF32_ST_BIND(sym->st_info)) {
                            frame.setSourceFileName(
                                      d_seg_p->d_helper_p->loadString(
                                           d_seg_p->d_stringTableSec.d_offset +
                                                          sourceFileNameOffset,
                                           d_scratchBuf_p,
                                           local::k_SCRATCH_BUF_LEN,
                                           &d_hbpAlloc));
                        }

                        frame.setMangledSymbolName(
                                  d_seg_p->d_helper_p->loadString(
                                           d_seg_p->d_stringTableSec.d_offset +
                                                                  sym->st_name,
                                           d_scratchBuf_p,
                                           local::k_SCRATCH_BUF_LEN,
                                           &d_hbpAlloc));
                        if (frame.isMangledSymbolNameKnown()) {
                            setFrameSymbolName(&frame);

                            it->setDone();

                            zprintf(
                                   "Resolved symbol %s, frame %ld, [%p, %p)\n",
                                   frame.symbolName().c_str(),
                                   &frame - &(*d_stackTrace_p)[0],
                                   symbolAddress,
                                   endSymbolAddress);

                            if (0 == --matched) {
                                zprintf("Last symbol in segment loaded\n");

                                return 0;                             // RETURN
                            }
                        }
                        else {
                            zprintf("Null symbol found for %p\n",
                                                                it->address());
                        }
                    }
                }
              }  break;
            }
        }
    }

    return 0;
}

int local::StackTraceResolver::processLoadedImage(
                                                 const char *fileName,
                                                 const void *programHeaders,
                                                 int         numProgramHeaders,
                                                 void       *textSegPtr,
                                                 void       *baseAddress)
    // note this must be public so 'linkMapCallBack' can call it on Solaris
{
    BSLS_ASSERT(!textSegPtr || !baseAddress);

    d_seg_p->reset();

#if defined(BSLS_PLATFORM_OS_HPUX)
    const char *name = fileName;
#else
    const char *name = 0;
    if (fileName && fileName[0]) {
        if (local::e_IS_LINUX) {
            d_seg_p->d_isMainExecutable = false;
        }

        name = fileName;
    }
    else {
        if (local::e_IS_LINUX) {
            d_seg_p->d_isMainExecutable = true;
        }

        // On Solaris and Linux, 'fileName' is sometimes null for the main
        // executable file, but those platforms have a standard virtual symlink
        // that points to the executable file name.

        const int numChars = static_cast<int>(
                                           readlink("/proc/self/exe",
                                                    d_scratchBuf_p,
                                                    local::k_SCRATCH_BUF_LEN));
        if (numChars > 0) {
            BSLS_ASSERT(numChars < local::k_SCRATCH_BUF_LEN);
            d_scratchBuf_p[numChars] = 0;
            name = d_scratchBuf_p;
        }
        else {
            zprintf("readlink of /proc/self/exe failed\n");

            return -1;                                                // RETURN
        }
    }
#endif
    name = bdlb::String::copy(name, &d_hbpAlloc);

    zprintf("processing loaded image: fn:\"%s\", name:\"%s\" main:%d"
                                                 " numHdrs:%d unmatched:%ld\n",
                        fileName ? fileName : "(null)", name ? name : "(null)",
                                 static_cast<int>(d_seg_p->d_isMainExecutable),
                               numProgramHeaders, d_seg_p->d_frameRecs.size());

    balst::StackTraceResolver_FileHelper helper(name);
    d_seg_p->d_helper_p = &helper;

    for (int i = 0; i < numProgramHeaders; ++i) {
        const local::ElfProgramHeader *ph =
              static_cast<const local::ElfProgramHeader *>(programHeaders) + i;
        // if (ph->p_type == PT_LOAD && ph->p_offset == 0) {

        if    (PT_LOAD == ph->p_type) {
            // on Linux, textSegPtr will be 0, on Solaris && HPUX, baseAddress
            // will be 0.  We will always have 1 of the two, and since they
            // differ by ph->p_vaddr, we can always calculate the one we don't
            // have.

            if (textSegPtr) {
                BSLS_ASSERT(0 == baseAddress);

                // calculating baseAddress from textSegPtr

                baseAddress = static_cast<char *>(textSegPtr) - ph->p_vaddr;
            }
            else {
                // or the other way around

                textSegPtr = static_cast<char *>(baseAddress) + ph->p_vaddr;
            }
            int rc = resolveSegment(baseAddress,    // base address
                                    textSegPtr,     // seg ptr
                                    ph->p_memsz,    // seg size
                                    name);          // file name
            if (rc) {
                return -1;                                            // RETURN
            }

            return 0;                                                 // RETURN
        }
    }

    return -1;
}

// PRIVATE ACCESSORS
void local::StackTraceResolver::setFrameSymbolName(
                                           balst::StackTraceFrame *frame) const
{
#if !defined(BSLS_PLATFORM_OS_SOLARIS)                                        \
 || defined(BSLS_PLATFORM_CMP_GNU) || defined(BSLS_PLATFORM_CMP_CLANG)
    // Linux or Sun g++ or HPUX

    int   status = -1;
    char *demangledSymbol = 0;
    if (d_demangle) {
        // note the demangler uses 'malloc' to allocate its result

#if defined(BSLS_PLATFORM_OS_HPUX)
        demangledSymbol = __cxa_demangle(frame->mangledSymbolName().c_str(),
                                         0,
                                         0,
                                         &status);
#else
        demangledSymbol = abi::__cxa_demangle(
                                            frame->mangledSymbolName().c_str(),
                                            0,
                                            0,
                                            &status);
#endif
    }
    if (0 == status) {
        zprintf("Demangled to: %s\n", demangledSymbol);
        frame->setSymbolName(demangledSymbol);
    }
    else {
        zprintf("Did not demangle: status: %d\n", status);
        frame->setSymbolName(frame->mangledSymbolName());
    }
    if (demangledSymbol) {
        bsl::free(demangledSymbol);
    }
#endif
#if defined(BSLS_PLATFORM_OS_SOLARIS)                                         \
 && !(defined(BSLS_PLATFORM_CMP_GNU) || defined(BSLS_PLATFORM_CMP_CLANG))
    // Sun CC only -- no demangling is available

    frame->setSymbolName(frame->mangledSymbolName());
#endif
}

#if defined(BSLS_PLATFORM_OS_LINUX)
// Linux could use the same method as Solaris, but we would need a special case
// for statically linked apps.  Instead of that we're going to use the
// 'dl_iterate_phdr' function, which works for static and dynamic apps (you get
// called back once if the app is static).

extern "C" {

static
int linkmapCallback(struct dl_phdr_info *info,
                    size_t               size,
                    void                *data)
    // This routine is called once for the executable file, and once for every
    // shared library that is loaded.  The specified 'info' contains a pointer
    // to the relevant information in the link map.  The specified 'size' is
    // unused, the specified 'data' is a pointer to the elf resolver.  Return 0
    // on success and a non-zero value otherwise.
{
    (void) size;

    local::StackTraceResolver *resolver =
                           reinterpret_cast<local::StackTraceResolver *>(data);

    // If we have completed resolving, there is no way to signal the caller to
    // stop iterating through the shared libs, but we aren't allowed to throw
    // and the caller ignores and propagates the return value we pass to it.
    // So just return without doing any work once resolving is done.

    if (0 == resolver->numUnmatchedFrames()) {
        return 0;                                                     // RETURN
    }

    // here the base address is known and text segment loading address is
    // unknown

    int rc = resolver->processLoadedImage(
                                    info->dlpi_name,
                                    info->dlpi_phdr,
                                    info->dlpi_phnum,
                                    0,
                                    reinterpret_cast<void *>(info->dlpi_addr));
    if (rc) {
        return -1;                                                    // RETURN
    }

    return 0;
}

}  // extern "C"

#endif
#if defined(BSLS_PLATFORM_OS_SOLARIS)

// modern Solaris applications are NEVER statically linked, so we always
// have a link_map.

extern "C" void *_DYNAMIC;    // global pointer that leads to the link map

#endif

// CLASS METHODS
int local::StackTraceResolver::resolve(
                                    balst::StackTrace *stackTrace,
                                    bool               demanglingPreferredFlag)
{
#if defined(BSLS_PLATFORM_OS_HPUX)

    int rc;

    local::StackTraceResolver resolver(stackTrace,
                                       demanglingPreferredFlag);

    // The HPUX compiler, 'aCC', doesn't accept the -Bstatic option, suggesting
    // we are never statically linked on HPUX, so 'shl_get_r' should always
    // work.

    shl_descriptor desc;
    bsl::memset(&desc, 0, sizeof(shl_descriptor));

    // 'programHeaders' will point to a segment of memory we will allocate and
    // reallocated to the needed size indicated by the 'ElfHeader's we
    // encounter.  The max is the number of program headers that will fit in
    // the allcoated segment.

    local::ElfProgramHeader *programHeaders = 0;
    int maxNumProgramHeaders = 0;

    local::ElfHeader elfHeader;
    for (int i = -1;
                0 < resolver.numUnmatchedFrames() && -1 != shl_get_r(i, &desc);
                                                                         ++i) {
        int numProgramHeaders = 0;

        {
            // this block limits the lifetime of 'helper' below

            zprintf("(%d) %s 0x%lx-0x%lx\n",
                    i,
                    desc.filename,
                    desc.tstart,
                    desc.tend);

            // index 0 is for the main executable

            resolver.d_seg_p->d_isMainExecutable = (0 == i);

            // note this will be opened twice, here and in 'processLoadedImage'

            balst::StackTraceResolver_FileHelper helper(desc.filename);

            rc = helper.readExact(&elfHeader, sizeof(elfHeader), 0);
            if (rc) {
                return -1;                                            // RETURN
            }

            numProgramHeaders = elfHeader.e_phnum;
            if (numProgramHeaders > maxNumProgramHeaders) {
                programHeaders = static_cast<local::ElfProgramHeader *>(
                       resolver.d_hbpAlloc.allocate(
                         numProgramHeaders * sizeof(local::ElfProgramHeader)));
                maxNumProgramHeaders = numProgramHeaders;
            }

            rc = helper.readExact(
                           programHeaders,
                           numProgramHeaders * sizeof(local::ElfProgramHeader),
                           elfHeader.e_phoff);
            if (rc) {
                return -1;                                            // RETURN
            }
        }

        rc = resolver.processLoadedImage(
                                   desc.filename,
                                   programHeaders,
                                   numProgramHeaders,
                                   static_cast<void *>(desc.tstart),
                                   0);
        if (rc) {
            return -1;                                                // RETURN
        }
    }

#elif defined(BSLS_PLATFORM_OS_LINUX)

    local::StackTraceResolver resolver(stackTrace,
                                       demanglingPreferredFlag);

    // 'dl_iterate_phdr' will iterate over all loaded files, the executable and
    // all the .so's.  It doesn't exist on Solaris.

    dl_iterate_phdr(&linkmapCallback,
                    &resolver);

#elif defined(BSLS_PLATFORM_OS_SOLARIS)

    local::StackTraceResolver resolver(stackTrace,
                                       demanglingPreferredFlag);

    struct link_map *linkMap = 0;

    // This method of getting the linkMap was deemed less desirable because it
    // calls 'malloc'.
    //
    //    dlinfo(RTLD_SELF, RTLD_DI_LINKMAP, &linkMap);
    //    if (0 == linkMap) {
    //        return -1;                                              // RETURN
    //    }

    if (0 == linkMap) {
        // This method was adopted as superior to the above (commented out)
        // method.

        local::ElfDynamic *dynamic = reinterpret_cast<local::ElfDynamic *>(
                                                                    &_DYNAMIC);
        if (0 == dynamic) {
            return -1;                                                // RETURN
        }

        while (true) {
            if (DT_DEBUG == dynamic->d_tag) {
                r_debug *rdb = reinterpret_cast<r_debug *>(
                                                          dynamic->d_un.d_ptr);
                if (rdb) {
                    linkMap = rdb->r_map;
                    break;
                }
                else {
                    return -1;                                        // RETURN
                }
            }
            else if (DT_NULL == dynamic->d_tag) {
                // end of list without finding the link map

                return -1;                                            // RETURN
            }

            ++dynamic;
        }
    }

    for (int i = 0; 0 < resolver.numUnmatchedFrames() && linkMap;
                                              ++i, linkMap = linkMap->l_next) {
        local::ElfHeader *elfHeader = reinterpret_cast<local::ElfHeader *>(
                                                              linkMap->l_addr);

        if (0 != checkElfHeader(elfHeader)) {
            return -1;
        }

        local::ElfProgramHeader *programHeaders =
            static_cast<local::ElfProgramHeader *>(static_cast<void *>(
                reinterpret_cast<char *>(elfHeader) + elfHeader->e_phoff));
        int numProgramHeaders = elfHeader->e_phnum;

        resolver.d_seg_p->d_isMainExecutable = (0 == i);

        // Here the text segment address is known, not base address.  On this
        // platform, but not necessarily on other platforms, the text segment
        // begins with the Elf Header.

        int rc = resolver.processLoadedImage(linkMap->l_name,
                                             programHeaders,
                                             numProgramHeaders,
                                             static_cast<void *>(elfHeader),
                                             0);
        if (rc) {
            return -1;                                                // RETURN
        }
    }

#else
# error unrecognized platform
#endif

    return 0;
}

// PUBLIC ACCESSOR
int local::StackTraceResolver::numUnmatchedFrames() const
{
    return static_cast<int>(d_seg_p->d_numTotalUnmatched);
}

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
