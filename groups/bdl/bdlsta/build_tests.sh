set -x
FLAGS="-filetype=o -O0 -stat"

PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../bdlb -I ../../bsl/bslim -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bsltf"

circle $FLAGS $PATHS bdlsta_linefit.t.cpp
circle $FLAGS $PATHS bdlsta_moment.t.cpp