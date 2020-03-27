set -x
PATHS="-I . -I ../bsls"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslscm_patchversion.t.cpp
circle $PATHS $FLAGS bslscm_versiontag.t.cpp
circle $PATHS $FLAGS bslscm_version.t.cpp
