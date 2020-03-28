set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bdl/bdlma -I ../../bdl/bdlat -I ../../bsl/bslmf -I ../../bdl/bdlscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bsla -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslalg -I ../../bsl/bslstp -I ../../bdl/bdlb -I ../../bsl/bslx -I ../../bdl/bdlf -I ../../bdl/bdlt -I ../../bdl/bdldfp -I ../../bdl/bdlsb -I ../../bdl/bdlde -I ../../../thirdparty/inteldfp/LIBRARY/src/"
FLAGS="-filetype=o -O0 -stat"

circle $FLAGS $PATHS balxml_base64parser.cpp
circle $FLAGS $PATHS balxml_configschema.cpp
circle $FLAGS $PATHS balxml_decoder.cpp
circle $FLAGS $PATHS balxml_decoderoptions.cpp
circle $FLAGS $PATHS balxml_elementattribute.cpp
circle $FLAGS $PATHS balxml_encoder.cpp
circle $FLAGS $PATHS balxml_encoderoptions.cpp
circle $FLAGS $PATHS balxml_encodingstyle.cpp
circle $FLAGS $PATHS balxml_errorinfo.cpp
circle $FLAGS $PATHS balxml_formatter.cpp
circle $FLAGS $PATHS balxml_hexparser.cpp
circle $FLAGS $PATHS balxml_listparser.cpp
circle $FLAGS $PATHS balxml_minireader.cpp
circle $FLAGS $PATHS balxml_namespaceregistry.cpp
circle $FLAGS $PATHS balxml_prefixstack.cpp
circle $FLAGS $PATHS balxml_reader.cpp
circle $FLAGS $PATHS balxml_typesparserutil.cpp
circle $FLAGS $PATHS balxml_typesprintutil.cpp
circle $FLAGS $PATHS balxml_util.cpp
circle $FLAGS $PATHS balxml_validatingreader.cpp
