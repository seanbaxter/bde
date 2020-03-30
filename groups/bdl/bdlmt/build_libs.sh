set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlcc -I ../../bsl/bslmt -I ../../bsl/bsl+bslhdrs -I ../bdlma -I ../../bsl/bslma -I ../../bsl/bslmf -I ../bdlt -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../bdlb -I ../bdlf"

circle $FLAGS $PATHS bdlmt_eventscheduler.cpp
circle $FLAGS $PATHS bdlmt_fixedthreadpool.cpp
circle $FLAGS $PATHS bdlmt_multiprioritythreadpool.cpp
circle $FLAGS $PATHS bdlmt_multiqueuethreadpool.cpp
circle $FLAGS $PATHS bdlmt_signaler.cpp
circle $FLAGS $PATHS bdlmt_threadmultiplexor.cpp
circle $FLAGS $PATHS bdlmt_threadpool.cpp
circle $FLAGS $PATHS bdlmt_throttle.cpp
circle $FLAGS $PATHS bdlmt_timereventscheduler.cpp
