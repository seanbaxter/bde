set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bslh_defaulthashalgorithm.cpp
circle $PATHS $FLAGS bslh_defaultseededhashalgorithm.cpp
circle $PATHS $FLAGS bslh_hash.cpp
circle $PATHS $FLAGS bslh_seededhash.cpp
circle $PATHS $FLAGS bslh_seedgenerator.cpp
circle $PATHS $FLAGS bslh_siphashalgorithm.cpp
circle $PATHS $FLAGS bslh_spookyhashalgorithm.cpp
circle $PATHS $FLAGS bslh_spookyhashalgorithmimp.cpp
