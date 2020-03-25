set -x
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm"
FLAGS="-filetype=o -O0 -stat"
circle $PATHS $FLAGS bosscm_version.cpp
circle $PATHS $FLAGS bosscm_versiontag.cpp
