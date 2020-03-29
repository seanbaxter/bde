set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlma -I ../../bsl/bslalg -I ../../bsl/bslmt -I ../../bsl/bslma -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslim -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../bdlb -I ../bdlf -I ../bdlc -I ../../bsl/bslx -I ../bdlcc -I ../bdlt -I ../../bsl/bsltf"

circle $PATHS $FLAGS bdlcc_boundedqueue.t.cpp
circle $PATHS $FLAGS bdlcc_cache.t.cpp
circle $PATHS $FLAGS bdlcc_deque.t.cpp
circle $PATHS $FLAGS bdlcc_fixedqueueindexmanager.t.cpp
circle $PATHS $FLAGS bdlcc_fixedqueue.t.cpp
circle $PATHS $FLAGS bdlcc_multipriorityqueue.t.cpp
circle $PATHS $FLAGS bdlcc_objectcatalog.t.cpp
circle $PATHS $FLAGS bdlcc_objectpool.t.cpp
circle $PATHS $FLAGS bdlcc_queue.t.cpp
circle $PATHS $FLAGS bdlcc_sharedobjectpool.t.cpp
circle $PATHS $FLAGS bdlcc_singleconsumerqueueimpl.t.cpp
circle $PATHS $FLAGS bdlcc_singleconsumerqueue.t.cpp
circle $PATHS $FLAGS bdlcc_singleproducerqueueimpl.t.cpp
circle $PATHS $FLAGS bdlcc_singleproducerqueue.t.cpp
circle $PATHS $FLAGS bdlcc_singleproducersingleconsumerboundedqueue.t.cpp
circle $PATHS $FLAGS bdlcc_skiplist.t.cpp
circle $PATHS $FLAGS bdlcc_stripedunorderedcontainerimpl.t.cpp
circle $PATHS $FLAGS bdlcc_stripedunorderedmap.t.cpp
circle $PATHS $FLAGS bdlcc_stripedunorderedmultimap.t.cpp
circle $PATHS $FLAGS bdlcc_timequeue.t.cpp
