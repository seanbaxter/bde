PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslalg -I ../../bsl/bslstl -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bdl/bdlscm -I ../../bdl/bdlb -I ../../bsl/bslmt -I ../../bsl/bslim -I ../../bdl/bdlsb -I ../../bsl/bslx -I ../../bsl/bsltf -I ../../bdl/bdls -I ../../bdl/bdlat -I ../../bdl/bdlde -I ../../bdl/bdlf -I ../../bdl/bdldfp -I ../../bdl/bdlma -I ../../../thirdparty/inteldfp/LIBRARY/src/"
FLAGS="-filetype=o -O0 -stat"
set -x

circle $FLAGS $PATHS balxml_base64parser.t.cpp
circle $FLAGS $PATHS balxml_configschema.t.cpp
circle $FLAGS $PATHS balxml_decoderoptions.t.cpp
circle $FLAGS $PATHS balxml_decoder.t.cpp
circle $FLAGS $PATHS balxml_elementattribute.t.cpp
circle $FLAGS $PATHS balxml_encoderoptions.t.cpp
circle $FLAGS $PATHS balxml_encoder.t.cpp
circle $FLAGS $PATHS balxml_encodingstyle.t.cpp
circle $FLAGS $PATHS balxml_errorinfo.t.cpp
circle $FLAGS $PATHS balxml_formatter.t.cpp
circle $FLAGS $PATHS balxml_hexparser.t.cpp
circle $FLAGS $PATHS balxml_listparser.t.cpp
circle $FLAGS $PATHS balxml_minireader.t.cpp
circle $FLAGS $PATHS balxml_namespaceregistry.t.cpp
circle $FLAGS $PATHS balxml_prefixstack.t.cpp
circle $FLAGS $PATHS balxml_reader.t.cpp
circle $FLAGS $PATHS balxml_typesparserutil.t.cpp
circle $FLAGS $PATHS balxml_typesprintutil.t.cpp
circle $FLAGS $PATHS balxml_util.t.cpp
circle $FLAGS $PATHS balxml_validatingreader.t.cpp
