set -x
PATHS="-I ."
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslfwd_bslma_allocator.t.cpp
circle $PATHS $FLAGS bslfwd_bslma_defaultallocatorguard.t.cpp
circle $PATHS $FLAGS bslfwd_bslma_default.t.cpp
circle $PATHS $FLAGS bslfwd_bslmf_nil.t.cpp
circle $PATHS $FLAGS bslfwd_bsls_asserttestexception.t.cpp
circle $PATHS $FLAGS bslfwd_bsls_stopwatch.t.cpp
circle $PATHS $FLAGS bslfwd_bsls_types.t.cpp
circle $PATHS $FLAGS bslfwd_buildtarget.t.cpp
