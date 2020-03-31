set -x
PATHS="-I ."
FLAGS="-O0 -filetype=o -fPIC -stat"

circle $PATHS $FLAGS bsls_alignedbuffer.cpp
circle $PATHS $FLAGS bsls_alignment.cpp
circle $PATHS $FLAGS bsls_alignmentfromtype.cpp
circle $PATHS $FLAGS bsls_alignmentimp.cpp
circle $PATHS $FLAGS bsls_alignmenttotype.cpp
circle $PATHS $FLAGS bsls_alignmentutil.cpp
circle $PATHS $FLAGS bsls_annotation.cpp
circle $PATHS $FLAGS bsls_assert.cpp
circle $PATHS $FLAGS bsls_assertimputil.cpp
circle $PATHS $FLAGS bsls_assert_macroreset.cpp
circle $PATHS $FLAGS bsls_asserttest.cpp
circle $PATHS $FLAGS bsls_asserttestexception.cpp
circle $PATHS $FLAGS bsls_asserttest_macroreset.cpp
circle $PATHS $FLAGS bsls_atomic.cpp
circle $PATHS $FLAGS bsls_atomicoperations_all_all_clangintrinsics.cpp
circle $PATHS $FLAGS bsls_atomicoperations_all_all_gccintrinsics.cpp
circle $PATHS $FLAGS bsls_atomicoperations_arm_all_gcc.cpp
circle $PATHS $FLAGS bsls_atomicoperations.cpp
circle $PATHS $FLAGS bsls_atomicoperations_default.cpp
circle $PATHS $FLAGS bsls_atomicoperations_ia64_hp_acc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_powerpc32_aix_xlc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_powerpc64_aix_xlc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_powerpc_aix_xlc_default.cpp
circle $PATHS $FLAGS bsls_atomicoperations_powerpc_all_gcc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_sparc32_sun_cc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_sparc64_sun_cc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_sparc_sun_cc_default.cpp
circle $PATHS $FLAGS bsls_atomicoperations_x64_all_gcc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_x64_win_msvc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_x86_all_gcc.cpp
circle $PATHS $FLAGS bsls_atomicoperations_x86_win_msvc.cpp
circle $PATHS $FLAGS bsls_blockgrowth.cpp
circle $PATHS $FLAGS bsls_bsldeprecationinformation.cpp
circle $PATHS $FLAGS bsls_bslexceptionutil.cpp
circle $PATHS $FLAGS bsls_bsllock.cpp
circle $PATHS $FLAGS bsls_bslonce.cpp
circle $PATHS $FLAGS bsls_bsltestutil.cpp
circle $PATHS $FLAGS bsls_buildtarget.cpp
circle $PATHS $FLAGS bsls_byteorder.cpp
circle $PATHS $FLAGS bsls_byteorderutil.cpp
circle $PATHS $FLAGS bsls_byteorderutil_impl.cpp
circle $PATHS $FLAGS bsls_compilerfeatures.cpp
circle $PATHS $FLAGS bsls_cpp11.cpp
circle $PATHS $FLAGS bsls_dbghelpdllimpl_windows.cpp
circle $PATHS $FLAGS bsls_deprecate.cpp
circle $PATHS $FLAGS bsls_exceptionutil.cpp
circle $PATHS $FLAGS bsls_ident.cpp
circle $PATHS $FLAGS bsls_int64.cpp
circle $PATHS $FLAGS bsls_keyword.cpp
circle $PATHS $FLAGS bsls_libraryfeatures.cpp
circle $PATHS $FLAGS bsls_linkcoercion.cpp
circle $PATHS $FLAGS bsls_log.cpp
circle $PATHS $FLAGS bsls_logseverity.cpp
circle $PATHS $FLAGS bsls_macroincrement.cpp
circle $PATHS $FLAGS bsls_macrorepeat.cpp
circle $PATHS $FLAGS bsls_nameof.cpp
circle $PATHS $FLAGS bsls_nativestd.cpp
circle $PATHS $FLAGS bsls_nullptr.cpp
circle $PATHS $FLAGS bsls_objectbuffer.cpp
circle $PATHS $FLAGS bsls_outputredirector.cpp
circle $PATHS $FLAGS bsls_performancehint.cpp
circle $PATHS $FLAGS bsls_platform.cpp
circle $PATHS $FLAGS bsls_platformutil.cpp
circle $PATHS $FLAGS bsls_pointercastutil.cpp
circle $PATHS $FLAGS bsls_protocoltest.cpp
circle $PATHS $FLAGS bsls_review.cpp
circle $PATHS $FLAGS bsls_review_macroreset.cpp
circle $PATHS $FLAGS bsls_spinlock.cpp
circle $PATHS $FLAGS bsls_stackaddressutil.cpp
circle $PATHS $FLAGS bsls_stopwatch.cpp
circle $PATHS $FLAGS bsls_systemclocktype.cpp
circle $PATHS $FLAGS bsls_systemtime.cpp
circle $PATHS $FLAGS bsls_timeinterval.cpp
circle $PATHS $FLAGS bsls_timeutil.cpp
circle $PATHS $FLAGS bsls_types.cpp
circle $PATHS $FLAGS bsls_unspecifiedbool.cpp
circle $PATHS $FLAGS bsls_util.cpp

ar rcs lib_bsls.a \
  bsls_alignedbuffer.o \
  bsls_alignment.o \
  bsls_alignmentfromtype.o \
  bsls_alignmentimp.o \
  bsls_alignmenttotype.o \
  bsls_alignmentutil.o \
  bsls_annotation.o \
  bsls_assert.o \
  bsls_assertimputil.o \
  bsls_assert_macroreset.o \
  bsls_asserttest.o \
  bsls_asserttestexception.o \
  bsls_asserttest_macroreset.o \
  bsls_atomic.o \
  bsls_atomicoperations_all_all_clangintrinsics.o \
  bsls_atomicoperations_all_all_gccintrinsics.o \
  bsls_atomicoperations_arm_all_gcc.o \
  bsls_atomicoperations.o \
  bsls_atomicoperations_default.o \
  bsls_atomicoperations_ia64_hp_acc.o \
  bsls_atomicoperations_powerpc32_aix_xlc.o \
  bsls_atomicoperations_powerpc64_aix_xlc.o \
  bsls_atomicoperations_powerpc_aix_xlc_default.o \
  bsls_atomicoperations_powerpc_all_gcc.o \
  bsls_atomicoperations_sparc32_sun_cc.o \
  bsls_atomicoperations_sparc64_sun_cc.o \
  bsls_atomicoperations_sparc_sun_cc_default.o \
  bsls_atomicoperations_x64_all_gcc.o \
  bsls_atomicoperations_x64_win_msvc.o \
  bsls_atomicoperations_x86_all_gcc.o \
  bsls_atomicoperations_x86_win_msvc.o \
  bsls_blockgrowth.o \
  bsls_bsldeprecationinformation.o \
  bsls_bslexceptionutil.o \
  bsls_bsllock.o \
  bsls_bslonce.o \
  bsls_bsltestutil.o \
  bsls_buildtarget.o \
  bsls_byteorder.o \
  bsls_byteorderutil.o \
  bsls_byteorderutil_impl.o \
  bsls_compilerfeatures.o \
  bsls_cpp11.o \
  bsls_dbghelpdllimpl_windows.o \
  bsls_deprecate.o \
  bsls_exceptionutil.o \
  bsls_ident.o \
  bsls_int64.o \
  bsls_keyword.o \
  bsls_libraryfeatures.o \
  bsls_linkcoercion.o \
  bsls_log.o \
  bsls_logseverity.o \
  bsls_macroincrement.o \
  bsls_macrorepeat.o \
  bsls_nameof.o \
  bsls_nativestd.o \
  bsls_nullptr.o \
  bsls_objectbuffer.o \
  bsls_outputredirector.o \
  bsls_performancehint.o \
  bsls_platform.o \
  bsls_platformutil.o \
  bsls_pointercastutil.o \
  bsls_protocoltest.o \
  bsls_review.o \
  bsls_review_macroreset.o \
  bsls_spinlock.o \
  bsls_stackaddressutil.o \
  bsls_stopwatch.o \
  bsls_systemclocktype.o \
  bsls_systemtime.o \
  bsls_timeinterval.o \
  bsls_timeutil.o \
  bsls_types.o \
  bsls_unspecifiedbool.o \
  bsls_util.o