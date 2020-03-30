set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslh -I ../../bsl/bslstp"

circle $FLAGS $PATHS bdlsb_fixedmeminput.cpp
circle $FLAGS $PATHS bdlsb_fixedmeminstreambuf.cpp
circle $FLAGS $PATHS bdlsb_fixedmemoutput.cpp
circle $FLAGS $PATHS bdlsb_fixedmemoutstreambuf.cpp
circle $FLAGS $PATHS bdlsb_memoutstreambuf.cpp
circle $FLAGS $PATHS bdlsb_overflowmemoutput.cpp
circle $FLAGS $PATHS bdlsb_overflowmemoutstreambuf.cpp
