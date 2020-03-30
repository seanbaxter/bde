set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslscm"

circle $FLAGS $PATHS bdlscm_version.cpp
circle $FLAGS $PATHS bdlscm_versiontag.cpp