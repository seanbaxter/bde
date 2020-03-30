set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlcc -I ../../bsl/bslmt -I ../../bsl/bsl+bslhdrs -I ../bdlma -I ../../bsl/bslma -I ../../bsl/bslmf -I ../bdlt -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../bdlb -I ../bdlf -I ../bdlsb -I ../../bsl/bslim -I ../../bsl/bsltf -I ../bdlc -I ../../bsl/bslx"

circle $FLAGS $PATHS bdlmt_eventscheduler.t.cpp
circle $FLAGS $PATHS bdlmt_fixedthreadpool.t.cpp
circle $FLAGS $PATHS bdlmt_multiprioritythreadpool.t.cpp
circle $FLAGS $PATHS bdlmt_multiqueuethreadpool.t.cpp
circle $FLAGS $PATHS bdlmt_signaler.t.cpp
circle $FLAGS $PATHS bdlmt_threadmultiplexor.t.cpp
circle $FLAGS $PATHS bdlmt_threadpool.t.cpp
circle $FLAGS $PATHS bdlmt_throttle.t.cpp
circle $FLAGS $PATHS bdlmt_timereventscheduler.t.cpp
