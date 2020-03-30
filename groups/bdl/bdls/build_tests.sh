set -x
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslim -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../bdlt -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../bdlb -I ../../bsl/bsltf -I ../../bsl/bslstp -I ../bdlf -I ../bdlsb -I ../bdlde -I ../../bsl/bslmt -I ../bdlma"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bdls_fdstreambuf.t.cpp
circle $PATHS $FLAGS bdls_filedescriptorguard.t.cpp
circle $PATHS $FLAGS bdls_filesystemutil.t.cpp
circle $PATHS $FLAGS bdls_memoryutil.t.cpp
circle $PATHS $FLAGS bdls_osutil.t.cpp
circle $PATHS $FLAGS bdls_pathutil.t.cpp
circle $PATHS $FLAGS bdls_pipeutil.t.cpp
circle $PATHS $FLAGS bdls_processutil.t.cpp
circle $PATHS $FLAGS bdls_testutil.t.cpp
