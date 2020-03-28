set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bdl/bdlt -I ../../bsl/bslmf -I ../../bdl/bdlma -I ../../bsl/bsla -I ../../bdl/bdlscm -I ../../bsl/bslma -I ../../bdl/bdlat -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlb -I ../../bsl/bslh -I ../../bdl/bdlb -I ../../bdl/bdldfp -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bdl/bdlc -I ../../bsl/bsltf -I ../balb -I ../../bdl/bdld -I ../../bsl/bslx -I ../../bdl/bdlsb -I ../../bdl/bdlde -I ../../bsl/bslmt -I ../balxml -I ../../bdl/bdlf -I ../../../thirdparty/inteldfp/LIBRARY/src/"

circle $PATHS $FLAGS baljsn_datumencoderoptions.t.cpp
circle $PATHS $FLAGS baljsn_datumutil.t.cpp
circle $PATHS $FLAGS baljsn_decoderoptions.t.cpp
circle $PATHS $FLAGS baljsn_decoder.t.cpp
circle $PATHS $FLAGS baljsn_encoderoptions.t.cpp
circle $PATHS $FLAGS baljsn_encoder.t.cpp
circle $PATHS $FLAGS baljsn_encodingstyle.t.cpp
circle $PATHS $FLAGS baljsn_formatter.t.cpp
circle $PATHS $FLAGS baljsn_parserutil.t.cpp
circle $PATHS $FLAGS baljsn_printutil.t.cpp
circle $PATHS $FLAGS baljsn_simpleformatter.t.cpp
circle $PATHS $FLAGS baljsn_tokenizer.t.cpp
