set -x
PATHS="-I . -I ../bsls"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslscm_patchversion.cpp
circle $PATHS $FLAGS bslscm_version.cpp
circle $PATHS $FLAGS bslscm_versiontag.cpp