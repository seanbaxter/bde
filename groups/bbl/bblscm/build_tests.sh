set -x
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bblscm_version.t.cpp
circle $PATHS $FLAGS bblscm_versiontag.t.cpp
