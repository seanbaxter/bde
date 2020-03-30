set -x
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslim -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../bdlt -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../bdlb -I ../../bsl/bsltf -I ../../bsl/bslstp -I ../bdlf -I ../bdlsb -I ../bdlde -I ../../bsl/bslmt"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bdls_fdstreambuf.cpp
circle $PATHS $FLAGS bdls_filedescriptorguard.cpp
circle $PATHS $FLAGS bdls_filesystemutil.cpp
circle $PATHS $FLAGS bdls_memoryutil.cpp
circle $PATHS $FLAGS bdls_osutil.cpp
circle $PATHS $FLAGS bdls_pathutil.cpp
circle $PATHS $FLAGS bdls_pipeutil.cpp
circle $PATHS $FLAGS bdls_processutil.cpp
circle $PATHS $FLAGS bdls_testutil.cpp
