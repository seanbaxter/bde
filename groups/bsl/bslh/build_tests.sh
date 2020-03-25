set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslh_defaulthashalgorithm.t.cpp
circle $PATHS $FLAGS bslh_defaultseededhashalgorithm.t.cpp
circle $PATHS $FLAGS bslh_hash.t.cpp
circle $PATHS $FLAGS bslh_seededhash.t.cpp
circle $PATHS $FLAGS bslh_seedgenerator.t.cpp
circle $PATHS $FLAGS bslh_siphashalgorithm.t.cpp
circle $PATHS $FLAGS bslh_spookyhashalgorithmimp.t.cpp
circle $PATHS $FLAGS bslh_spookyhashalgorithm.t.cpp
