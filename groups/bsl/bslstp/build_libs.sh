set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla -I ../bslalg -I ../bslma -I ../bslstl -I ../bslh"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslstp_alloc.cpp
circle $PATHS $FLAGS bslstp_exalgorithm.cpp
circle $PATHS $FLAGS bslstp_exfunctional.cpp
circle $PATHS $FLAGS bslstp_hash.cpp
circle $PATHS $FLAGS bslstp_hashmap.cpp
circle $PATHS $FLAGS bslstp_hashset.cpp
circle $PATHS $FLAGS bslstp_hashtable.cpp
circle $PATHS $FLAGS bslstp_iterator.cpp
circle $PATHS $FLAGS bslstp_slistbase.cpp
circle $PATHS $FLAGS bslstp_slist.cpp
circle $PATHS $FLAGS bslstp_util.cpp
