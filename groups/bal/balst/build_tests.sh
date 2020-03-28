
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslmt -I ../../bdl/bdlmt -I ../../bdl/bdlma -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlscm -I ../../bsl/bslstp -I ../../bsl/bslh -I ../../bdl/bdls -I ../../bdl/bdlt -I ../../bdl/bdlb -I ../ball -I ../../bsl/bslim -I ../../bsl/bslx -I ../../bsl/bsltf -I ../../bdl/bdlsb -I ../../bdl/bdlcc"
FLAGS="-filetype=o -O0 -stat"
set -x
circle $PATHS $FLAGS balst_assertionlogger.t.cpp
circle $PATHS $FLAGS balst_objectfileformat.t.cpp
circle $PATHS $FLAGS balst_stacktraceframe.t.cpp
circle $PATHS $FLAGS balst_stacktraceprintutil.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolver_dwarfreader.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolver_filehelper.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolverimpl_dladdr.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolverimpl_elf.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolverimpl_windows.t.cpp
circle $PATHS $FLAGS balst_stacktraceresolverimpl_xcoff.t.cpp
circle $PATHS $FLAGS balst_stacktrace.t.cpp
circle $PATHS $FLAGS balst_stacktracetestallocator.t.cpp
circle $PATHS $FLAGS balst_stacktraceutil.t.cpp
