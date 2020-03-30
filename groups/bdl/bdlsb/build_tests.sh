set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslh -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bsl/bslalg -I ../../bsl/bsltf -I ../../bsl/bslx"

circle $FLAGS $PATHS bdlsb_fixedmeminput.t.cpp
circle $FLAGS $PATHS bdlsb_fixedmeminstreambuf.t.cpp
circle $FLAGS $PATHS bdlsb_fixedmemoutput.t.cpp
circle $FLAGS $PATHS bdlsb_fixedmemoutstreambuf.t.cpp
circle $FLAGS $PATHS bdlsb_memoutstreambuf.t.cpp
circle $FLAGS $PATHS bdlsb_overflowmemoutput.t.cpp
circle $FLAGS $PATHS bdlsb_overflowmemoutstreambuf.t.cpp