set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslalg -I ../../bsl/bslscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../balscm -I ../../bsl/bslma -I ../../bdl/bdlat -I ../../bdl/bdlscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmt -I ../../bsl/bslstl -I ../../bdl/bdlb -I ../../bsl/bslh -I ../../bdl/bdlmt -I ../../bsl/bslx -I ../../bsl/bslstp -I ../../bdl/bdlcc -I ../../bsl/bslim -I ../../bdl/bdls -I ../../bdl/bdlma -I ../../bdl/bdlt -I ../../bsl/bsltf -I ../balb -I ../../bdl/bdlf"

circle $FLAGS $PATHS balb_controlmanager.t.cpp
circle $FLAGS $PATHS balb_filecleanerconfiguration.t.cpp
circle $FLAGS $PATHS balb_filecleanerutil.t.cpp
circle $FLAGS $PATHS balb_performancemonitor.t.cpp
circle $FLAGS $PATHS balb_pipecontrolchannel.t.cpp
circle $FLAGS $PATHS balb_testmessages.t.cpp
