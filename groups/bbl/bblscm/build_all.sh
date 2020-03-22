set -x
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bblscm_version.cpp
circle $PATHS $FLAGS bblscm_versiontag.cpp
