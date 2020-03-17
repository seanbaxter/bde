set -x
PATHS="-I . -I ../../bsl/bsls -I ../../bsl/bslalg -I ../../bsl/bslscm -I ../balscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslmt -I ../../bdl/bdlat -I ../../bdl/bdlscm -I ../../bsl/bsl+bslhdrs/ -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bdl/bdlb -I ../../bsl/bslx -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bdl/bdlde -I ../../bdl/bdlf -I ../../bsl/bslim -I ../../bdl/bdlma -I ../../bal/balb -I ../../bdl/bdls -I ../../bdl/bdlmt -I ../../bdl/bdlcc"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS balb_controlmanager.cpp
circle $PATHS $FLAGS balb_filecleanerconfiguration.cpp
circle $PATHS $FLAGS balb_filecleanerutil.cpp
circle $PATHS $FLAGS balb_performancemonitor.cpp
circle $PATHS $FLAGS balb_pipecontrolchannel.cpp
circle $PATHS $FLAGS balb_testmessages.cpp
