set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bslfwd_bslma_allocator.cpp
circle $PATHS $FLAGS bslfwd_bslma_defaultallocatorguard.cpp
circle $PATHS $FLAGS bslfwd_bslma_default.cpp
circle $PATHS $FLAGS bslfwd_bslmf_nil.cpp
circle $PATHS $FLAGS bslfwd_bsls_asserttestexception.cpp
circle $PATHS $FLAGS bslfwd_bsls_stopwatch.cpp
circle $PATHS $FLAGS bslfwd_bsls_types.cpp
circle $PATHS $FLAGS bslfwd_buildtarget.cpp
