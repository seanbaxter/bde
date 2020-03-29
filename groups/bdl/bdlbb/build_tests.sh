set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bslma -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslalg -I ../bdlb -I ../../bsl/bslstp -I ../bdlma -I ../../bsl/bslmt -I ../../bsl/bslim -I ../../bsl/bsltf -I ../bdlt -I ../../bsl/bslx -I ../bdlsb"

circle $FLAGS $PATHS bdlbb_blob.t.cpp
circle $FLAGS $PATHS bdlbb_blobstreambuf.t.cpp
circle $FLAGS $PATHS bdlbb_blobutil.t.cpp
circle $FLAGS $PATHS bdlbb_pooledblobbufferfactory.t.cpp
circle $FLAGS $PATHS bdlbb_simpleblobbufferfactory.t.cpp
