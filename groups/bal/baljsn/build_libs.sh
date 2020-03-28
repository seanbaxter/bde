set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bdl/bdlma -I ../../bdl/bdlscm -I ../../bsl/bslmf -I ../../bdl/bdlt -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslma -I ../../bsl/bslstl -I ../../bdl/bdlat -I ../../bsl/bslh -I ../../bdl/bdldfp -I ../../bdl/bdlb -I ../../bsl/bslstp -I ../../bdl/bdlde -I ../../bdl/bdlc -I ../../bsl/bslim -I ../../bdl/bdld -I ../../bdl/bdlsb -I ../../bsl/bslx -I ../../../thirdparty/inteldfp/LIBRARY/src"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS baljsn_datumencoderoptions.cpp
circle $PATHS $FLAGS baljsn_datumutil.cpp
circle $PATHS $FLAGS baljsn_decoder.cpp
circle $PATHS $FLAGS baljsn_decoderoptions.cpp
circle $PATHS $FLAGS baljsn_encoder.cpp
circle $PATHS $FLAGS baljsn_encoderoptions.cpp
circle $PATHS $FLAGS baljsn_encodingstyle.cpp
circle $PATHS $FLAGS baljsn_formatter.cpp
circle $PATHS $FLAGS baljsn_parserutil.cpp
circle $PATHS $FLAGS baljsn_printutil.cpp
circle $PATHS $FLAGS baljsn_simpleformatter.cpp
circle $PATHS $FLAGS baljsn_tokenizer.cpp
