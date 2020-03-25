set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdls -I ../../bsl/bslmf -I ../../bsl/bslmt -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlma -I ../../bsl/bslh -I ../../bdl/bdlscm -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bdl/bdlb -I ../../bsl/bslim -I ../ball -I ../../bsl/bslx -I ../../bdl/bdlsb -I ../../bdl/bdlcc"

circle $FLAGS $PATHS balst_assertionlogger.cpp
circle $FLAGS $PATHS balst_objectfileformat.cpp
circle $FLAGS $PATHS balst_stacktrace.cpp
circle $FLAGS $PATHS balst_stacktraceframe.cpp
circle $FLAGS $PATHS balst_stacktraceprintutil.cpp
circle $FLAGS $PATHS balst_stacktraceresolver_dwarfreader.cpp
circle $FLAGS $PATHS balst_stacktraceresolver_filehelper.cpp
circle $FLAGS $PATHS balst_stacktraceresolverimpl_dladdr.cpp
circle $FLAGS $PATHS balst_stacktraceresolverimpl_elf.cpp
circle $FLAGS $PATHS balst_stacktraceresolverimpl_windows.cpp
circle $FLAGS $PATHS balst_stacktraceresolverimpl_xcoff.cpp
circle $FLAGS $PATHS balst_stacktracetestallocator.cpp
circle $FLAGS $PATHS balst_stacktraceutil.cpp
