set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslstl -I ../bslmf -I ../bsla -I ../bslma -I ../bslh -I ../bslalg"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslstp_alloc.t.cpp
circle $PATHS $FLAGS bslstp_exalgorithm.t.cpp
circle $PATHS $FLAGS bslstp_exfunctional.t.cpp
circle $PATHS $FLAGS bslstp_hashmap.t.cpp
circle $PATHS $FLAGS bslstp_hashset.t.cpp
circle $PATHS $FLAGS bslstp_hashtable.t.cpp
circle $PATHS $FLAGS bslstp_hash.t.cpp
circle $PATHS $FLAGS bslstp_iterator.t.cpp
circle $PATHS $FLAGS bslstp_slistbase.t.cpp
circle $PATHS $FLAGS bslstp_slist.t.cpp
circle $PATHS $FLAGS bslstp_util.t.cpp
