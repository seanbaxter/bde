set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlat -I ../../bdl/bdlscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdlb -I ../../bsl/bslh -I ../../bsl/bslx -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bdl/bdldfp -I ../../bdl/bdlsb -I ../../bsl/bslmt -I ../../../thirdparty/inteldfp/LIBRARY/src"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS balber_berconstants.cpp
circle $PATHS $FLAGS balber_berdecoder.cpp
circle $PATHS $FLAGS balber_berdecoderoptions.cpp
circle $PATHS $FLAGS balber_berencoder.cpp
circle $PATHS $FLAGS balber_berencoderoptions.cpp
circle $PATHS $FLAGS balber_beruniversaltagnumber.cpp
circle $PATHS $FLAGS balber_berutil.cpp