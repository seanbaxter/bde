set -x
PATHS="-I . -I ../../bsl/bsls -I ../bosscm -I ../../bsl/bslscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs"
FLAGS="-filetype=o -O0 -stat"
circle $FLAGS $PATHS bostst_map.cpp