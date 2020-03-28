set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslim -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bsltf"

circle $FLAGS $PATHS balscm_versiontag.t.cpp
circle $FLAGS $PATHS balscm_version.t.cpp
