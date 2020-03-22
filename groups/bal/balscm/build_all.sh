set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm"

circle $FLAGS $PATHS balscm_version.cpp
circle $FLAGS $PATHS balscm_versiontag.cpp