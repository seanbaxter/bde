set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlma -I ../../bsl/bslalg -I ../../bsl/bslmt -I ../../bsl/bslma -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslim -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../bdlb -I ../bdlf -I ../bdlc -I ../../bsl/bslx"

circle $FLAGS $PATHS bdlcc_boundedqueue.cpp
circle $FLAGS $PATHS bdlcc_cache.cpp
circle $FLAGS $PATHS bdlcc_deque.cpp
circle $FLAGS $PATHS bdlcc_fixedqueue.cpp
circle $FLAGS $PATHS bdlcc_fixedqueueindexmanager.cpp
circle $FLAGS $PATHS bdlcc_multipriorityqueue.cpp
circle $FLAGS $PATHS bdlcc_objectcatalog.cpp
circle $FLAGS $PATHS bdlcc_objectpool.cpp
circle $FLAGS $PATHS bdlcc_queue.cpp
circle $FLAGS $PATHS bdlcc_sharedobjectpool.cpp
circle $FLAGS $PATHS bdlcc_singleconsumerqueue.cpp
circle $FLAGS $PATHS bdlcc_singleconsumerqueueimpl.cpp
circle $FLAGS $PATHS bdlcc_singleproducerqueue.cpp
circle $FLAGS $PATHS bdlcc_singleproducerqueueimpl.cpp
circle $FLAGS $PATHS bdlcc_singleproducersingleconsumerboundedqueue.cpp
circle $FLAGS $PATHS bdlcc_skiplist.cpp
circle $FLAGS $PATHS bdlcc_stripedunorderedcontainerimpl.cpp
circle $FLAGS $PATHS bdlcc_stripedunorderedmap.cpp
circle $FLAGS $PATHS bdlcc_stripedunorderedmultimap.cpp
circle $FLAGS $PATHS bdlcc_timequeue.cpp
