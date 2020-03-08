set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla -I ../bsl+bslhdrs -I ../bslstl -I ../bslh -I ../bslalg -I ../bslma -I ../bslim"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslx_byteinstream.cpp
circle $PATHS $FLAGS bslx_byteoutstream.cpp
circle $PATHS $FLAGS bslx_genericinstream.cpp
circle $PATHS $FLAGS bslx_genericoutstream.cpp
circle $PATHS $FLAGS bslx_instreamfunctions.cpp
circle $PATHS $FLAGS bslx_marshallingutil.cpp
circle $PATHS $FLAGS bslx_outstreamfunctions.cpp
circle $PATHS $FLAGS bslx_streambufinstream.cpp
circle $PATHS $FLAGS bslx_streambufoutstream.cpp
circle $PATHS $FLAGS bslx_testinstream.cpp
circle $PATHS $FLAGS bslx_testinstreamexception.cpp
circle $PATHS $FLAGS bslx_testoutstream.cpp
circle $PATHS $FLAGS bslx_typecode.cpp
circle $PATHS $FLAGS bslx_versionfunctions.cpp
