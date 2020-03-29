set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bslma -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslalg -I ../bdlb -I ../../bsl/bslstp -I ../bdlma -I ../../bsl/bslmt"

circle $FLAGS $PATHS bdlbb_blob.cpp
circle $FLAGS $PATHS bdlbb_blobstreambuf.cpp
circle $FLAGS $PATHS bdlbb_blobutil.cpp
circle $FLAGS $PATHS bdlbb_pooledblobbufferfactory.cpp
circle $FLAGS $PATHS bdlbb_simpleblobbufferfactory.cpp
