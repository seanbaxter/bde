set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsl+bslhdrs -I ../bsla -I ../bslstl -I ../bslh -I ../bslalg -I ../bslma"
FLAGS="-filetype=o -O0 -stat"

circle $FLAGS $PATHS bslx_byteinstream.t.cpp
circle $FLAGS $PATHS bslx_byteoutstream.t.cpp
circle $FLAGS $PATHS bslx_genericinstream.t.cpp
circle $FLAGS $PATHS bslx_genericoutstream.t.cpp
circle $FLAGS $PATHS bslx_instreamfunctions.t.cpp
circle $FLAGS $PATHS bslx_marshallingutil.t.cpp
circle $FLAGS $PATHS bslx_outstreamfunctions.t.cpp
circle $FLAGS $PATHS bslx_streambufinstream.t.cpp
circle $FLAGS $PATHS bslx_streambufoutstream.t.cpp
circle $FLAGS $PATHS bslx_testinstreamexception.t.cpp
circle $FLAGS $PATHS bslx_testinstream.t.cpp
circle $FLAGS $PATHS bslx_testoutstream.t.cpp
circle $FLAGS $PATHS bslx_typecode.t.cpp
circle $FLAGS $PATHS bslx_versionfunctions.t.cpp
