// bslstl_stringbuf.h                                                 -*-C++-*-
#ifndef INCLUDED_BSLSTL_STRINGBUF
#define INCLUDED_BSLSTL_STRINGBUF

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a C++03-compatible 'stringbuf' class.
//
//@CLASSES:
//  bsl::stringbuf: C++03-compatible 'stringbuf' class
//  bsl::StringBufContainer: wrapper for 'basic_stringbuf'
//
//@SEE_ALSO: bslstl_stringstream, bslstl_ostringstream, bslstl_istringstream
//
//@DESCRIPTION: This component is for internal use only.  Please include
// '<bsl_sstream.h>' instead.
//
// This component defines a class template, 'bsl::basic_stringbuf', that
// implements a standard string buffer, providing an unformatted character
// input sequence and an unformatted character output sequence that
// may be initialized or accessed using a string value (see 27.8.2 [stringbuf]
// of the C++11 standard).  This component also defines two standard aliases,
// 'bsl::stringbuf' and 'bsl::wstringbuf', that refer to specializations of the
// 'bsl::basic_stringbuf' template for 'char' and 'wchar_t' types,
// respectively.  As with any stream buffer class, 'bsl::basic_stringbuf' is
// rarely used directly.  Stream buffers provide low-level unformatted
// input/output.  They are usually plugged into 'std::basic_stream' classes to
// provide higher-level formatted input and output via 'operator>>' and
// 'operator<<'.  'bsl::basic_stringbuf' is used in the
// 'bsl::basic_stringstream' family of classes and users should prefer those
// classes over direct use of 'bsl::basic_stringbuf'.
//
// 'bsl::basic_stringbuf' derives from 'std::basic_streambuf' and implements
// the necessary protected virtual methods.  In this way 'bsl::basic_stringbuf'
// customizes the behavior of 'std::basic_streambuf' to redirect the reading
// and writing of characters to an internally-maintained sequence of characters
// that can be initialized or accessed using a 'bsl::basic_string'.  Note that
// although the standard mandates functions that access and modify the
// buffered sequence using a 'basic_string', it does not mandate that a
// 'basic_stringbuf' internally store this buffer in a 'basic_string'; this
// implementation currently uses a 'basic_string' as its internal buffer, but
// that is subject to change without warning.
//
// The 'bsl::stringbuf' template has three parameters, 'CHAR_TYPE',
// 'CHAR_TRAITS', and 'ALLOCATOR'.  The 'CHAR_TYPE' and 'CHAR_TRAITS'
// parameters respectively define the character type for the stream buffer and
// a type providing a set of operations the stream buffer will use to
// manipulate characters of that type, which must meet the character traits
// requirements defined by the C++11 standard, 21.2 [char.traits].  The
// 'ALLOCATOR' template parameter is described in the "Memory Allocation"
// section below.
//
///Memory Allocation
///-----------------
// The type supplied as a stream buffer's 'ALLOCATOR' template parameter
// determines how that stream buffer will allocate memory.  The
// 'basic_stringbuf' template supports allocators meeting the requirements
// of the C++11 standard, 17.6.3.5 [allocator.requirements]; in addition, it
// supports scoped-allocators derived from the 'bslma::Allocator' memory
// allocation protocol.  Clients intending to use 'bslma'-style allocators
// should use 'bsl::allocator', which provides a C++11 standard-compatible
// adapter for a 'bslma::Allocator' object.  Note that the standard aliases
// 'bsl::stringbuf' and 'bsl::wstringbuf' both use 'bsl::allocator'.
//
///'bslma'-Style Allocators
/// - - - - - - - - - - - -
// If the type supplied for the 'ALLOCATOR' template parameter of a 'stringbuf'
// instantiation is 'bsl::allocator', then objects of that stream buffer type
// will conform to the standard behavior of a 'bslma'-allocator-enabled type.
// Such a stream buffer accepts an optional 'bslma::Allocator' argument at
// construction.  If the address of a 'bslma::Allocator' object is explicitly
// supplied at construction, it will be used to supply memory for the stream
// buffer throughout its lifetime; otherwise, the stream buffer will use the
// default allocator installed at the time of the stream buffer's construction
// (see 'bslma_default').
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic Operations
///- - - - - - - - - - - - - -
// The following example demonstrates the use of 'bsl::stringbuf' to read and
// write character data from and to a 'bsl::string' object.
//
// Suppose we want to implement a simplified converter from 'unsigned int' to
// 'bsl::string' and back.  First, we define the prototypes of two conversion
// functions:
//..
//  bsl::string  toString(unsigned int from);
//  unsigned int fromString(const bsl::string& from);
//..
// Then, we use 'bsl::stringbuf' to implement the 'toString' function.  We
// write all digits into 'bsl::stringbuf' individually using 'sputc' methods
// and then return the resulting 'bsl::string' object:
//..
//  #include <algorithm>
//
//  bsl::string toString(unsigned int from)
//  {
//      bsl::stringbuf out;
//
//      for (; from != 0; from /= 10) {
//          out.sputc('0' + from % 10);
//      }
//
//      bsl::string result(out.str());
//      std::reverse(result.begin(), result.end());
//      return result;
//  }
//..
// Now, we implement the 'fromString' function that converts from
// 'bsl::string' to 'unsigned int' by using 'bsl::stringbuf' to read individual
// digits from the string object:
//..
//  unsigned int fromString(const bsl::string& from)
//  {
//      unsigned int result = 0;
//
//      for (bsl::stringbuf in(from); in.in_avail(); ) {
//          result = result * 10 + (in.sbumpc() - '0');
//      }
//
//      return result;
//  }
//..
// Finally, we verify that the result of the round-trip conversion is identical
// to the original value:
//..
//  unsigned int orig   = 92872498;
//  unsigned int result = fromString(toString(orig));
//
//  assert(orig == result);
//..

// Prevent 'bslstl' headers from being included directly in 'BSL_OVERRIDES_STD'
// mode.  Doing so is unsupported, and is likely to cause compilation errors.
#if defined(BSL_OVERRIDES_STD) && !defined(BOS_STDHDRS_PROLOGUE_IN_EFFECT)
#error "include <bsl_sstream.h> instead of <bslstl_stringbuf.h> in \
BSL_OVERRIDES_STD mode"
#endif
#include <bslscm_version.h>

#include <bslstl_iosfwd.h>
#include <bslstl_string.h>

#include <bslalg_swaputil.h>

#include <bslma_stdallocator.h>
#include <bslma_usesbslmaallocator.h>

#include <bsls_assert.h>
#include <bsls_libraryfeatures.h>

#include <algorithm>
#include <cstddef>
#include <ios>
#include <istream>
#include <ostream>
#include <streambuf>

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
# include <utility>
#endif
#include <limits.h>      // for 'INT_MAX', 'INT_MIN'

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
#ifndef BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#include <bslalg_typetraits.h>
#endif // BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#endif // BDE_OMIT_INTERNAL_DEPRECATED

namespace BloombergLP {
// 'BSLS_ASSERT' filename fix -- See {'bsls_assertimputil'}
#ifdef BSLS_ASSERTIMPUTIL_AVOID_STRING_CONSTANTS
extern const char s_bslstl_stringbuf_h[];
#undef BSLS_ASSERTIMPUTIL_FILE
#define BSLS_ASSERTIMPUTIL_FILE BloombergLP::s_bslstl_stringbuf_h
#endif
}

namespace bsl {

using native_std::ios_base;

                            // =====================
                            // class basic_stringbuf
                            // =====================

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
class basic_stringbuf
    : public native_std::basic_streambuf<CHAR_TYPE, CHAR_TRAITS> {
    // This class implements a standard stream buffer providing an unformatted
    // character input sequence and an unformatted character output sequence
    // that may be initialized or accessed using a string value.

  private:
    // PRIVATE TYPES
    typedef native_std::basic_streambuf<CHAR_TYPE, CHAR_TRAITS>  BaseType;
    typedef bsl::basic_string<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR> StringType;

  public:
    // TYPES
    typedef CHAR_TYPE                      char_type;
    typedef CHAR_TRAITS                    traits_type;
    typedef ALLOCATOR                      allocator_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::off_type off_type;
    typedef typename traits_type::pos_type pos_type;

  private:
    // DATA
    StringType         d_str;   // internal character sequence buffer

    off_type           d_endHint;
                                // offset to one past the last known good
                                // character in 'd_str'  (Note that to enable
                                // efficient buffering, 'd_str' may be resized
                                // beyond the last written character, so
                                // 'd_str.size()' may not accurately report
                                // the current length of the character
                                // sequence available for input.  Extending
                                // the size of 'd_str' and updating 'epptr'
                                // (the end-of-output pointer) allows the
                                // parent stream type to write additional
                                // characters without 'overflow'.  However,
                                // care must be taken to refresh the cached
                                // 'd_endHint' value as the parent stream will
                                // update the current output position 'pptr',
                                // without calling a method on this type.)

    ios_base::openmode d_mode;  // 'stringbuf' open mode ('in', 'out', or both)

  private:
    // NOT IMPLEMENTED
    basic_stringbuf(const basic_stringbuf&);             // = delete
    basic_stringbuf& operator=(const basic_stringbuf&);  // = delete

  private:
    // PRIVATE MANIPULATORS
    pos_type updateInputPointers(char_type *currentInputPosition);
        // Update the input pointers ('eback', 'gptr', 'egptr') of this string
        // buffer, setting the beginning of the input sequence, 'eback', to the
        // address of the first character of the internal string
        // representation, 'd_str', the current position of the input sequence,
        // 'gptr', to the specified 'currentInputPosition', and the address
        // past the end of the accessible sequence, 'egptr', to the last
        // character in 'd_ptr' ('&d_ptr[0] + d_endHint').  Return the offset
        // of the current position of the input sequence from the start of the
        // sequence.  The behavior is undefined unless this buffer is in input
        // mode and 'currentInputPosition' is within the range of accessible
        // characters in 'd_ptr'.

    pos_type updateOutputPointers(char_type *currentOutputPosition);
        // Update the output pointers ('pback', 'pptr', 'epptr') of this string
        // buffer, setting the beginning of the output sequence, 'pback', to
        // the address of the first character of the internal string
        // representation, 'd_str', the current position of the output
        // sequence, 'pptr', to the specified 'currentOutputPosition', and the
        // address past the end of the accessible sequence, 'epptr', to one
        // past the last character in 'd_ptr' ('&d_ptr[0] + d_ptr.size()').
        // Return the offset of the current position of the output sequence
        // from the start of the sequence.  The behavior is undefined unless
        // this buffer is in output mode, and 'currentOutputPosition' is within
        // the range of accessible characters in 'd_ptr'.

    void updateStreamPositions(off_type inputOffset  = 0,
                               off_type outputOffset = 0);
        // Update the input and output positions of this string buffer object
        // according to the current state of the internal string representation
        // 'd_ptr'.  Optionally specify an 'inputOffset' indicating the current
        // input position's offset from the beginning of the sequence.
        // Optionally specify an 'outputOffset' indicating the current output
        // position's offset from the beginning of the sequence.  If this
        // buffer is in output mode, set the beginning of the output sequence,
        // 'pback', to the address of the first character of 'd_ptr', the
        // current output position, 'pptr', to 'pback + outputOffset', and the
        // end of the output sequence, 'epptr', to one past the last character
        // in 'd_str' ('&d_ptr[0] + d_ptr.size()').  If this buffer is in input
        // mode, set the beginning of the input sequence, 'eback', to the
        // address of the first character of 'd_ptr', the current input
        // position, 'gptr', to 'eback + inputOffset', and the end of the input
        // sequence, 'egptr', to the last written character in 'd_str'
        // ('&d_ptr[0] + d_endHint').

    bool extendInputArea();
        // Attempt to expand the sequence of characters available for input
        // (i.e., update the end of input buffer position, 'egptr') to
        // incorporate additional characters that may have been written (as
        // output) to the stream.  Return 'true' if the input buffer was
        // successfully extended, and 'false' otherwise.  Note that the input
        // area as described by 'eback', 'gptr', and 'egptr' may become out of
        // sync with the characters actually available in the buffer as the
        // parent 'basic_streambuf' type may perform writes into the output
        // area (using 'pbase', 'pptr', and 'epptr') without calling any
        // methods of this object.

    // PRIVATE ACCESSORS
    pos_type streamSize() const;
        // Return the number of characters currently in the buffer.  Note this
        // may not be 'd_str.size()', as this implementation resizes 'd_str'
        // beyond the number of written characters to provide more efficient
        // buffering, and it also may not be 'd_endHint', as that value may
        // currently be stale (as writes may have been performed through the
        // parent 'basic_streambuf' type without calling a method on this
        // object).

    bool arePointersValid(const char_type *first,
                          const char_type *middle,
                          const char_type *last) const;
        // Return 'true' if pointers form a valid range
        // ('first <= middle <= last') and 'first == d_str.data()' and
        // 'middle' and  'last' are in the range
        // '[d_str.data() .. d_str.data() + d_str.size()]', or all arguments
        // are 0, and 'false' otherwise.  Note that this function is called in
        // defensive (i.e., "DEBUG" or "SAFE") build modes only.

  protected:
    // PROTECTED MANIPULATORS
    virtual pos_type seekoff(
                off_type           offset,
                ios_base::seekdir  whence,
                ios_base::openmode modeBitMask = ios_base::in | ios_base::out);
        // Set the current input position or the current output position (or
        // both) to the specified 'offset' from the specified 'whence'
        // location.  Optionally specify a 'modeBitMask' indicating whether to
        // set the current input position, current output position, or both.
        // If 'whence' is 'ios_base::beg', set the current position to the
        // indicated 'offset' from the beginning of the stream; if 'whence' is
        // 'ios_base::end', set the current position to the indicated 'offset'
        // from the end of the stream; and if 'whence' is 'ios_base::cur', set
        // the current input or output position to the indicated 'offset' from
        // its current position.  If 'whence' is 'ios_base::cur', then
        // 'modeBitMask' may be either 'ios_base::in' or 'ios_base::out', but
        // not both.  Return the offset of the new position on success, and
        // 'pos_type(off_type(-1))' otherwise.

    virtual pos_type seekpos(
             pos_type           absoluteOffset,
             ios_base::openmode modeBitMask = ios_base::in | ios_base::out);
        // Set the current input position or the current output position (or
        // both) to the specified 'absoluteOffset' from the beginning of the
        // stream.  Optionally specify a 'modeBitMask' indicating whether to
        // set the current input position, current output position, or both.
        // Return the offset of the new position on success, and
        // 'pos_type(off_type(-1))' otherwise.

    virtual native_std::streamsize xsgetn(
                                        char_type              *result,
                                        native_std::streamsize  numCharacters);
        // Read up to the specified 'numCharacters' from this 'stringbuf'
        // object and store them in the specified 'result' array.  Return the
        // number of characters loaded into 'result'.  Note that if fewer than
        // 'numCharacters' characters are available in the buffer, all
        // available characters are loaded into 'result'.  The behavior is
        // undefined unless 'result' refers to a contiguous sequence of at
        // least 'numCharacters' characters.

    virtual int_type underflow();
        // Return the character at the current input position, if a character
        // is available, and 'traits_type::eof()' otherwise.   Update the end
        // of the input area, 'egptr', if additional characters are available
        // (as may occur if additional characters have been written to the
        // string buffer).  Note that this operation is similar to 'uflow',
        // but does not advance the current input position.

    virtual int_type uflow();
        // Return the character at the current input position and advance the
        // input position by 1.  If no character is available at the current
        // input position, return 'traits_type::eof()'.   Update the end of the
        // input area, 'egptr', if additional characters are available (as may
        // occur if additional characters have been written to the string
        // buffer).  Note that this operation is similar to 'underflow', but
        // advances the current input position.

    virtual int_type pbackfail(int_type character = traits_type::eof());
        // Put back the specified 'character' into the input sequence so that
        // the next character read from the input sequence will be
        // 'character'.  If 'character' is either 'traits_type::eof()' or is
        // the same as the previously read character from the input sequence,
        // then adjust the current input position, 'gptr', back one position.
        // If 'character' is neither 'traits_type::eof()' nor the character
        // previously read from the input sequence, but this string buffer was
        // opened for writing ('ios_base::out'), then adjust the input
        // sequence back one position and write 'character' to that position.
        // Return the character that was put back on success and
        // 'traits_type::eof()' if either the input position is currently at
        // the beginning of the sequence or if the previous character in the
        // input sequence is not 'character' and this buffer was not opened
        // for writing.

    virtual native_std::streamsize xsputn(
                                        const char_type        *inputString,
                                        native_std::streamsize  numCharacters);
        // Append the specified 'numCharacters' from the specified
        // 'inputString' to the output sequence starting at the current output
        // position ('pptr').  Update the current output position of this
        // string buffer to refer to the last appended character.  Return the
        // number of characters that were appended.  The behavior is undefined
        // unless 'inputString' refers to a contiguous sequence of at least
        // 'numCharacters' characters.

    virtual int_type overflow(int_type character = traits_type::eof());
        // Append the specified 'character' to the output sequence of this
        // stream buffer at the current output position ('pptr'), and advance
        // the output position by one.  This operation may update the end of
        // output area ('epptr') to allow for additional writes (e.g., by the
        // base 'basic_streambuf' type) to the output sequence without calling
        // a method on this type.  Return the written character on success, and
        // 'traits_type::eof()' if 'character' is 'traits_type::eof()' or this
        // stream buffer was not opened for writing.

  public:
    // CREATORS
    explicit
    basic_stringbuf(const allocator_type& allocator = allocator_type());
    explicit
    basic_stringbuf(ios_base::openmode    modeBitMask,
                    const allocator_type& allocator = allocator_type());
    explicit
    basic_stringbuf(const StringType&     initialString,
                    const allocator_type& allocator = allocator_type());
    basic_stringbuf(const StringType&     initialString,
                    ios_base::openmode    modeBitMask,
                    const allocator_type& allocator = allocator_type());
        // Create a 'basic_stringbuf' object.  Optionally specify a
        // 'modeBitMask' indicating whether this buffer may be read from,
        // written to, or both.  If 'modeBitMask' is not supplied, this buffer
        // is created with 'ios_base::in | ios_base::out'.  Optionally specify
        // an 'initialString' indicating the initial sequence of characters
        // that this buffer will access or manipulate.  If 'initialString' is
        // not supplied, the initial sequence of characters will be empty.
        // Optionally specify the 'allocator' used to supply memory.  If
        // 'allocator' is not supplied, a default-constructed object of the
        // (template parameter) 'ALLOCATOR' type is used.  If the 'ALLOCATOR'
        // argument is of type 'bsl::allocator' (the default), then
        // 'allocator', if supplied, shall be convertible to
        // 'bslma::Allocator *'.  If the 'ALLOCATOR' argument is of type
        // 'bsl::allocator' and 'allocator' is not supplied, the currently
        // installed default allocator will be used to supply memory.

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
    basic_stringbuf(basic_stringbuf&&     original);
    basic_stringbuf(basic_stringbuf&&     original,
                    const allocator_type& allocator);
        // Create a 'basic_stringbuf' object having the same value as the
        // specified 'original' object by moving the contents of 'original' to
        // the newly-created object.  Optionally specify the 'allocator' used
        // to supply memory.  'original' is left in a valid but unspecified
        // state.
#endif

    ~basic_stringbuf();
        // Destroy this object.

    // MANIPULATORS
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
    basic_stringbuf& operator=(basic_stringbuf&& rhs);
        // Assign to this object the value of the specified 'rhs', and return a
        // reference providing modifiable access to this object.  The contents
        // of 'rhs' are move-assigned to this object.  'rhs' is left in a valid
        // but unspecified state.
#endif

    void str(const StringType& value);
        // Reset the internally buffered sequence of characters to the
        // specified 'value'.  Update the beginning and end of both the input
        // and output sequences to be the beginning and end of the updated
        // buffer, update the current input position to be the beginning of the
        // updated buffer, and update the current output position to be the end
        // of the updated buffer.

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY
    void swap(basic_stringbuf& other);
        // Efficiently exchange the value of this object with the value of the
        // specified 'other' object.  This method provides the no-throw
        // exception-safety guarantee if '*this' and 'other' allocators compare
        // equal.  The behavior is undefined unless either '*this' and 'other'
        // allocators compare equal or 'propagate_on_container_swap' is 'true'.
        // Note that this function is only available for C++11 (and later)
        // language standards because it requires that 'swap' be provided on
        // the (platform supplied) base class for this type.
#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY

    // ACCESSORS
    allocator_type get_allocator() const BSLS_KEYWORD_NOEXCEPT;
        // Return the allocator used by the underlying string to supply memory.

    StringType str() const;
        // Return the currently buffered sequence of characters.  If this
        // object was created only in input mode, the resultant 'StringType'
        // contains the character sequence in the range '[eback(), egptr())'.
        // If 'modeBitMask & ios_base::out' specified at construction is
        // nonzero then the resultant 'StringType' contains the character
        // sequence in the range '[pbase(), high_mark)', where 'high_mark'
        // represents the position one past the highest initialized character
        // in the buffer.  Otherwise this object has been created in neither
        // input nor output mode and a zero length 'StringType' is returned.
};

// FREE FUNCTIONS
#if defined(BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY)                  \
 && defined(BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE)
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
void swap(basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>& a,
          basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>& b);
    // Efficiently exchange the values of the specified 'a' and 'b' objects.
    // This method provides the no-throw exception-safety guarantee if 'a' and
    // 'b' allocators compare equal.  Note that this function is only available
    // for C++11 (and later) language standards.
#endif

// STANDARD TYPEDEFS
typedef basic_stringbuf<char, char_traits<char>, allocator<char> >   stringbuf;
typedef basic_stringbuf<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
                                                                    wstringbuf;

                      // =========================
                      // struct StringBufContainer
                      // =========================

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
class StringBufContainer {
    // This class enables the implementation of string-stream types by
    // providing a trivial type containing a 'basic_stringbuf' that is suitable
    // as a ('private') base class for a string-stream.  Inheriting from
    // 'StringBufContainer' allows the string-stream to ensure that the
    // contained 'basic_stringbuf' is initialized before initializing other
    // base classes or data members without potentially overriding 'virtual'
    // methods in the 'basic_stringbuf' type.  Note that implementations of
    // string-stream types must pass the address of a string-buffer to their
    // 'public' base class (e.g., 'basic_stream'), so the string-stream must
    // ensure (using 'private' inheritance) that the string-buffer is
    // initialized before constructing the 'public' base class.  If a
    // string-stream implementation were to directly inherit from
    // 'basic_streambuf', then 'virtual' methods defined in that string-stream
    // (e.g., 'underflow') might incorrectly override those in the
    // 'basic_stringbuf' implementation.

  private:
    // PRIVATE TYPES
    typedef basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>   StreamBufType;
    typedef bsl::basic_string<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR> StringType;

    // DATA
    StreamBufType d_bufObj;  // contained 'basic_stringbuf'

  private:
    // NOT IMPLEMENTED
    StringBufContainer(const StringBufContainer&);             // = delete
    StringBufContainer& operator=(const StringBufContainer&);  // = delete

  public:
    // CREATORS
    explicit
    StringBufContainer(const ALLOCATOR& allocator)
    : d_bufObj(allocator)
    {
    }

    StringBufContainer(ios_base::openmode modeBitMask,
                       const ALLOCATOR&   allocator)
    : d_bufObj(modeBitMask, allocator)
    {
    }

    StringBufContainer(const StringType& initialString,
                       const ALLOCATOR&  allocator)
    : d_bufObj(initialString, allocator)
    {
    }

    StringBufContainer(const StringType&  initialString,
                       ios_base::openmode modeBitMask,
                       const ALLOCATOR&   allocator)
    : d_bufObj(initialString, modeBitMask, allocator)
    {
    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
    StringBufContainer(StringBufContainer&& original)
        // Create a 'StringBufContainer' object having the same value as the
        // specified 'original' object by moving the contents of 'original' to
        // the newly-created object.  'original' is left in a valid but
        // unspecified state.
    : d_bufObj(std::move(original.d_bufObj))
    {
    }

    StringBufContainer(StringBufContainer&& original,
                       const ALLOCATOR&     allocator)
    // Create a 'StringBufContainer' object using the specified 'allocator' to
    // supply memory and having the same value as the specified 'original'
    // object by moving the contents of 'original' to the newly-created object.
    // 'original' is left in a valid but unspecified state.
    : d_bufObj(std::move(original.d_bufObj), allocator)
    {
    }
#endif

    //! ~StringBufContainer() = default;

    // MANIPULATORS
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
    StringBufContainer& operator=(StringBufContainer&& rhs)
        // Assign to this object the value of the specified 'rhs', and return a
        // reference providing modifiable access to this object.  The contents
        // of 'rhs' are move-assigned to this object.  'rhs' is left in a valid
        // but unspecified state.
    {
        d_bufObj = std::move(rhs.d_bufObj);

        return *this;
    }
#endif

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY
    void swap(StringBufContainer& other)
        // Efficiently exchange the value of this object with the value of the
        // specified 'other' object.
    {
        d_bufObj.swap(other.d_bufObj);
    }
#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY

    // ACCESSORS
    StreamBufType *rdbuf() const
    {
        return const_cast<StreamBufType *>(&d_bufObj);
    }
};

// ============================================================================
//                       TEMPLATE FUNCTION DEFINITIONS
// ============================================================================

                            // ---------------------
                            // class basic_stringbuf
                            // ---------------------

// PRIVATE MANIPULATORS
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pos_type
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
                           updateInputPointers(char_type *currentInputPosition)
{
    BSLS_ASSERT(d_mode & ios_base::in);
    BSLS_ASSERT(&d_str[0] <= currentInputPosition);
    BSLS_ASSERT(currentInputPosition <=
                        &d_str[0] + static_cast<std::ptrdiff_t>(streamSize()));

    char_type *dataPtr = &d_str[0];

    this->setg(dataPtr,
               currentInputPosition,
               dataPtr + static_cast<std::ptrdiff_t>(streamSize()));
    return pos_type(currentInputPosition - dataPtr);
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pos_type
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
                         updateOutputPointers(char_type *currentOutputPosition)
{
    BSLS_ASSERT(d_mode & ios_base::out);
    BSLS_ASSERT(&d_str[0] <= currentOutputPosition);
    BSLS_ASSERT(currentOutputPosition <= &d_str[0] + d_str.size());

    char_type          *dataPtr  = &d_str[0];
    native_std::size_t  dataSize =  d_str.size();

    pos_type outputPos = currentOutputPosition - dataPtr;
    this->setp(dataPtr, dataPtr + dataSize);
    pos_type bumpAmount = outputPos;
    while (bumpAmount > INT_MAX) {
        this->pbump(INT_MAX);
        bumpAmount -= INT_MAX;
    }
    if (bumpAmount) {
        this->pbump(int(bumpAmount));
    }
    return outputPos;
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
void basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    updateStreamPositions(off_type inputOffset, off_type outputOffset)
{
    // Extend the internal buffer to the full capacity of the string to allow
    // us to use the full capacity for buffering output.

    d_str.resize(d_str.capacity());
    char_type *dataPtr = &d_str[0];

    if (d_mode & ios_base::out) {
        // Update the output position.

        native_std::size_t dataSize = d_str.size();
        this->setp(dataPtr, dataPtr + dataSize);
        off_type bumpAmount = outputOffset;
        while (bumpAmount < INT_MIN) {
            this->pbump(INT_MIN);
            bumpAmount -= INT_MIN;
        }
        while (bumpAmount > INT_MAX) {
            this->pbump(INT_MAX);
            bumpAmount -= INT_MAX;
        }
        if (bumpAmount) {
            this->pbump(int(bumpAmount));
        }
    }

    if (d_mode & ios_base::in) {
        // Update the input position.

        this->setg(dataPtr,
                   dataPtr + inputOffset,
                   dataPtr + static_cast<std::ptrdiff_t>(streamSize()));
    }
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
bool basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::extendInputArea()
{
    // Try to extend into written buffer.

    if (d_mode & ios_base::out && this->pptr() > this->egptr()) {
        d_endHint = streamSize();
        updateInputPointers(this->gptr());
        return true;                                                  // RETURN
    }

    return false;
}

// PRIVATE ACCESSORS
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pos_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::streamSize() const
{
    pos_type size = native_std::max<off_type>(d_endHint,
                                              this->pptr() - this->pbase());

    BSLS_ASSERT(size <= off_type(d_str.size()));

    return size;
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
bool basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::arePointersValid(
                                                  const char_type *first,
                                                  const char_type *middle,
                                                  const char_type *last) const
{
    const bool isNull            = first == 0;
    const char_type *bufferBegin = isNull ? 0 : d_str.data();
    const char_type *bufferEnd   = isNull ? 0 : d_str.data() + d_str.size();
    return first  == bufferBegin
        && last   <= bufferEnd
        && first  <= middle
        && middle <= last;
}

// PROTECTED MANIPULATORS
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pos_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::seekoff(
                                               off_type           offset,
                                               ios_base::seekdir  whence,
                                               ios_base::openmode modeBitMask)
{
    // If 'whence' is 'ios_base::cur' (the current position), 'modeBitMask'
    // may not be both input and output mode.

    if (((modeBitMask & (ios_base::in | ios_base::out)) ==
                        (ios_base::in | ios_base::out))
        && whence == ios_base::cur) {
        return pos_type(off_type(-1));                                // RETURN
    }

    pos_type newPos = pos_type(off_type(-1));

    // Set the current input position.

    if ((modeBitMask & ios_base::in) && (d_mode & ios_base::in)) {
        char_type *inputPtr = 0;

        switch (whence) {
          case ios_base::beg: {
            inputPtr = this->eback() + offset;
          } break;
          case ios_base::cur: {
            inputPtr = this->gptr() + offset;
          } break;
          case ios_base::end: {
            inputPtr = this->eback()
                       + static_cast<std::ptrdiff_t>(streamSize())
                       + offset;
          } break;
          default: {
            BSLS_ASSERT_OPT(false && "invalid seekdir argument");
          }
        }

        if (inputPtr < this->eback()
         || inputPtr >
                   this->eback() + static_cast<std::ptrdiff_t>(streamSize())) {
            // 'inputPtr' is outside the valid range of the string buffer.

            return pos_type(off_type(-1));                            // RETURN
        }

        newPos = updateInputPointers(inputPtr);
    }

    // Set the current output position.

    if ((modeBitMask & ios_base::out) && (d_mode & ios_base::out)) {
        char_type *outputPtr = 0;

        switch (whence) {
          case ios_base::beg: {
            outputPtr = this->pbase() + offset;
          } break;
          case ios_base::cur: {
            outputPtr = this->pptr() + offset;
          } break;
          case ios_base::end: {
            outputPtr = this->pbase()
                      + static_cast<std::ptrdiff_t>(streamSize())
                      + offset;
          } break;
          default:
            BSLS_ASSERT_OPT(false && "invalid seekdir argument");
        }

        if (outputPtr < this->pbase()
         || outputPtr >
                   this->pbase() + static_cast<std::ptrdiff_t>(streamSize())) {
            // 'outputPtr' is outside the valid range of the string buffer.

            return pos_type(off_type(-1));                            // RETURN
        }

        newPos = updateOutputPointers(outputPtr);
    }

    return newPos;
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pos_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::seekpos(
                                             pos_type           absoluteOffset,
                                             ios_base::openmode modeBitMask)
{
    return basic_stringbuf::seekoff(off_type(absoluteOffset),
                                    ios_base::beg,
                                    modeBitMask);
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
native_std::streamsize
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::xsgetn(
                                          char_type              *result,
                                          native_std::streamsize numCharacters)
{
    if ((d_mode & ios_base::in) == 0) {
        return 0;                                                     // RETURN
    }

    // Additional characters may become available for reading when the input
    // area is extended to account for any characters newly written to the
    // output sequence.
    extendInputArea();

    if (this->gptr() != this->egptr()) {
        // There are characters available in this buffer.

        native_std::streamsize available = this->egptr() - this->gptr();
        native_std::streamsize readChars = native_std::min(available,
                                                           numCharacters);

        traits_type::copy(result, this->gptr(), native_std::size_t(readChars));
        this->gbump(int(readChars));

        return readChars;                                             // RETURN
    }

    return 0;
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::int_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::underflow()
{
    if ((d_mode & ios_base::in) == 0) {
        return traits_type::eof();                                    // RETURN
    }

    if (this->gptr() != this->egptr()) {
        // There are characters available in this buffer.

        return traits_type::to_int_type(*this->gptr());               // RETURN
    }

    if (extendInputArea()) {
        // Additional characters may become available after the input area is
        // extended.

        return this->basic_stringbuf::underflow();                    // RETURN
    }

    return traits_type::eof();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::int_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::uflow()
{
    if ((d_mode & ios_base::in) == 0) {
        return traits_type::eof();                                    // RETURN
    }

    if (this->gptr() != this->egptr()) {
        // There are characters available in this buffer.

        int_type c = traits_type::to_int_type(*this->gptr());
        this->gbump(1);
        return c;                                                     // RETURN
    }

    if (extendInputArea()) {
        // Additional characters may become available after the input area is
        // extended.

        return this->basic_stringbuf::uflow();                        // RETURN
    }

    return traits_type::eof();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::int_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::pbackfail(
                                                            int_type character)
{
    if (this->gptr() == this->eback()) {
        // The current position is at the start of the buffer, so we cannot
        // push back a character.

        return traits_type::eof();                                    // RETURN
    }

    if (traits_type::eq_int_type(character, traits_type::eof())
     || traits_type::eq_int_type(
                              character,
                              traits_type::to_int_type(*(this->gptr() - 1)))) {
        // If 'character' is 'eof' or the previous input character,
        // simply move the current position back 1.

        this->gbump(-1);
        return traits_type::to_int_type(*this->gptr());               // RETURN
    }

    if (d_mode & ios_base::out) {
        // In output mode, if 'character' is not the previous input character,
        // overwrite the previous input character.

        this->gbump(-1);
        *this->gptr() = traits_type::to_char_type(character);
        return character;                                             // RETURN
    }

    return traits_type::eof();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
native_std::streamsize
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::xsputn(
                                         const char_type        *inputString,
                                         native_std::streamsize  numCharacters)
{
    if ((d_mode & ios_base::out) == 0) {
        return 0;                                                     // RETURN
    }
    BSLS_ASSERT(this->pptr());
    BSLS_ASSERT(this->pbase());

    // Compute the space required.

    native_std::streamsize spaceLeft   =
                                    d_str.data() + d_str.size() - this->pptr();
    native_std::ptrdiff_t toOverwrite =
              native_std::ptrdiff_t(native_std::min(spaceLeft, numCharacters));

    // Append the portion of 'inputString' that can be written without
    // resizing 'd_ptr'.

    traits_type::copy(this->pptr(), inputString, toOverwrite);

    off_type inputOffset = this->gptr() - this->eback();

    if (numCharacters == toOverwrite) {
        // If all of 'inputString' has been written, just update the stream
        // positions.

        off_type newHigh = numCharacters + this->pptr() - this->pbase();
        d_endHint        = native_std::max(d_endHint, newHigh);

        updateStreamPositions(inputOffset, newHigh);
    }
    else {
        // If some characters remain to be written, append them to 'd_str'
        // (resizing 'd_str' in the process).

        d_str.append(inputString + toOverwrite, inputString + numCharacters);

        // Update the last written character cache, and the input stream
        // positions.

        d_endHint = d_str.size();
        updateStreamPositions(inputOffset, d_endHint);
    }

    return numCharacters;
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::int_type
    basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::overflow(
                                                            int_type character)
{
    if ((d_mode & ios_base::out) == 0) {
        return traits_type::eof();                                    // RETURN
    }

    if (traits_type::eq_int_type(character, traits_type::eof())) {
        // Nothing to write, so just return success.

        return traits_type::not_eof(character);                       // RETURN
    }

    char_type c = traits_type::to_char_type(character);
    if (this->pptr() != this->epptr()) {
        // Additional space is available in 'd_str', so no need to resize the
        // buffer.

        *this->pptr() = c;
        this->pbump(1);

        d_endHint = streamSize();
    }
    else {
        // Store the input offset so it can be used to restore the input and
        // output positions after the next resize.

        off_type inputOffset = this->gptr() - this->eback();

        // Append the character, and expand the buffer.

        d_str.push_back(c);

        // Update the input sequence, restoring the current input position
        // from 'inputOffset', and updating the output sequence to reflect the
        // newly resized buffer.

        d_endHint = d_str.size();
        updateStreamPositions(inputOffset, d_endHint);
    }

    return character;
}

// CREATORS
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(const allocator_type& allocator)
: BaseType()
, d_str(allocator)
, d_endHint(0)
, d_mode(ios_base::in | ios_base::out)
{
    updateStreamPositions();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(ios_base::openmode    modeBitMask,
                    const allocator_type& allocator)
: BaseType()
, d_str(allocator)
, d_endHint(0)
, d_mode(modeBitMask)
{
    updateStreamPositions();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(const StringType&     initialString,
                    const allocator_type& allocator)
: BaseType()
, d_str(initialString, allocator)
, d_endHint(initialString.size())
, d_mode(ios_base::in | ios_base::out)
{
    updateStreamPositions();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(const StringType&     initialString,
                    ios_base::openmode    modeBitMask,
                    const allocator_type& allocator)
: BaseType()
, d_str(initialString, allocator)
, d_endHint(initialString.size())
, d_mode(modeBitMask)
{
    updateStreamPositions(0, d_mode & ios_base::ate ? d_endHint : 0);
}

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(basic_stringbuf&& original)
: BaseType()
, d_str(std::move(original.d_str))
, d_endHint(std::move(original.d_endHint))
, d_mode(std::move(original.d_mode))
{
    // Capture the positions for later restoration

    const off_type  inputOffset = original.gptr() - original.eback();
    const off_type outputOffset = original.pptr() - original.pbase();
    updateStreamPositions(inputOffset, outputOffset);

    this->pubimbue(original.getloc());

    if (original.d_endHint > 0 &&
        static_cast<size_t>(original.d_endHint) > original.d_str.size()) {

        // The move has moved away the string

        original.d_endHint = 0;
        original.updateStreamPositions();
    }
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    basic_stringbuf(basic_stringbuf&&     original,
                    const allocator_type& allocator)
: BaseType()
, d_str(std::move(original.d_str), allocator)
, d_endHint(std::move(original.d_endHint))
, d_mode(std::move(original.d_mode))
{
    // Capture the positions for later restoration

    const off_type inputOffset  = original.gptr() - original.eback();
    const off_type outputOffset = original.pptr() - original.pbase();
    updateStreamPositions(inputOffset, outputOffset);

    this->pubimbue(original.getloc());

    if (original.d_endHint > 0 &&
        static_cast<size_t>(original.d_endHint) > original.d_str.size()) {

        // The move has moved away the string

        original.d_endHint = 0;
        original.updateStreamPositions();
    }
}
#endif

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    ~basic_stringbuf()
{
    if (d_mode & ios_base::in) {
        BSLS_ASSERT(arePointersValid(this->eback(),
                                     this->gptr(),
                                     this->egptr()));
    }

    if (d_mode & ios_base::out) {
        BSLS_ASSERT(arePointersValid(this->pbase(),
                                     this->pptr(),
                                     this->epptr()));
    }
}

// MANIPULATORS
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>&
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::
    operator=(basic_stringbuf&& rhs)
{
    // Capture the positions for later restoration

    const off_type inputOffset  = rhs.gptr()  - rhs.eback();
    const off_type inputSize    = rhs.egptr() - rhs.eback();
    const off_type outputOffset = rhs.pptr()  - rhs.pbase();
    const off_type outputSize   = rhs.epptr() - rhs.pbase();

    this->pubimbue(rhs.getloc());

    d_str     = std::move(rhs.d_str);
    d_endHint = std::move(rhs.d_endHint);
    d_mode    = std::move(rhs.d_mode);

    // Fix the stream-position pointers

    char_type *dataPtr = &d_str[0];

    // Update positions/pointers in the moved-to object

    this->setp(dataPtr, dataPtr + outputSize);
    this->pbump(static_cast<int>(outputOffset));
    this->setg(dataPtr,
               dataPtr + inputOffset,
               dataPtr + inputSize);

    // Reset positions/pointers in the moved-from object

    if (rhs.d_endHint > 0 &&
        static_cast<size_t>(rhs.d_endHint) > rhs.d_str.size()) {

        // The move has moved away the string

        rhs.d_endHint = 0;
        rhs.updateStreamPositions();
    }

    return *this;
}
#endif

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
void basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::str(
                                                       const StringType& value)
{
    d_str     = value;
    d_endHint = d_str.size();
    updateStreamPositions(0, d_mode & ios_base::ate ? d_endHint : 0);
}

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
void basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::swap(
                                                        basic_stringbuf& other)
{
    BSLS_ASSERT(
         bsl::allocator_traits<ALLOCATOR>::propagate_on_container_swap::value
      || d_str.get_allocator() == other.d_str.get_allocator());
    // Capture the positions for the later restoration.  Formally,
    // 'std::basic_streambuf::swap' exchanges the internal pointers and the
    // locale object.  But 'bsl::string' swapping can invalidate pointers, so
    // we need to control this process manually.

    const off_type           inputOffset       = this->gptr() - this->eback();
    const off_type           outputOffset      = this->pptr() - this->pbase();
    const native_std::locale loc               = this->getloc();

    const off_type           otherInputOffset  = other.gptr() - other.eback();
    const off_type           otherOutputOffset = other.pptr() - other.pbase();
    const native_std::locale otherLoc          = other.getloc();

    // Parent method invocation.

    this->BaseType::swap(other);

    // Swapping data members.

    this->pubimbue(otherLoc);
    other.pubimbue(loc);

    d_str.swap(other.d_str);
    BloombergLP::bslalg::SwapUtil::swap(&this->d_endHint, &other.d_endHint);
    BloombergLP::bslalg::SwapUtil::swap(&this->d_mode,    &other.d_mode);

    // Fix the stream-position pointers.

    this->updateStreamPositions(otherInputOffset, otherOutputOffset);
    other.updateStreamPositions(     inputOffset,      outputOffset);
}
#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY

// ACCESSORS
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
typename basic_stringbuf<CHAR_TYPE,CHAR_TRAITS,ALLOCATOR>::allocator_type
basic_stringbuf<CHAR_TYPE,CHAR_TRAITS,ALLOCATOR>::get_allocator() const
                                                          BSLS_KEYWORD_NOEXCEPT
{
    return d_str.get_allocator();
}

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
inline
typename basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::StringType
basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>::str() const
{
    if (d_mode & ios_base::out) {
        return StringType(d_str.begin(),
                          d_str.begin()
                          + static_cast<std::ptrdiff_t>(streamSize()));
                                                                      // RETURN

    }

    if (d_mode & ios_base::in) {
        return StringType(this->eback(), this->egptr());              // RETURN
    }

    return StringType();
}

}  // close namespace bsl

// FREE FUNCTIONS
#if defined(BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY)                  \
 && defined(BSLS_LIBRARYFEATURES_HAS_CPP11_STREAM_MOVE)
template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
void bsl::swap(basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>& a,
               basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR>& b)
{
    typedef BloombergLP::bslmf::MovableRefUtil MoveUtil;

    if (a.get_allocator() == b.get_allocator()
    ||  bsl::allocator_traits<ALLOCATOR>::propagate_on_container_swap::value) {
        a.swap(b);
    }
    else {
        basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR> aCopy(
                                                            MoveUtil::move(a),
                                                            b.get_allocator());
        basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR> bCopy(
                                                            MoveUtil::move(b),
                                                            a.get_allocator());
        swap(a, bCopy);
        swap(b, aCopy);
    }
}
#endif

// ============================================================================
//                                TYPE TRAITS
// ============================================================================

namespace BloombergLP {
namespace bslma {

template <class CHAR_TYPE, class CHAR_TRAITS, class ALLOCATOR>
struct UsesBslmaAllocator<
        bsl::basic_stringbuf<CHAR_TYPE, CHAR_TRAITS, ALLOCATOR> >
    : bsl::true_type
{};

}  // close namespace bslma
}  // close enterprise namespace

// Undo 'BSLS_ASSERT' filename fix -- See {'bsls_assertimputil'}
#ifdef BSLS_ASSERTIMPUTIL_AVOID_STRING_CONSTANTS
#undef BSLS_ASSERTIMPUTIL_FILE
#define BSLS_ASSERTIMPUTIL_FILE BSLS_ASSERTIMPUTIL_DEFAULTFILE
#endif

#endif

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
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
