set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs"

circle $FLAGS $PATHS bdlsta_linefit.cpp
circle $FLAGS $PATHS bdlsta_moment.cpp