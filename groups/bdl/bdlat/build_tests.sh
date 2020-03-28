set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslx -I ../bdldfp -I ../../bsl/bslstp -I ../bdlt -I ../../bsl/bslim -I ../../bsl/bsltf -I ../../../thirdparty/inteldfp/LIBRARY/src/"

circle $FLAGS $PATHS bdlat_arrayfunctions.t.cpp
circle $FLAGS $PATHS bdlat_arrayiterators.t.cpp
circle $FLAGS $PATHS bdlat_attributeinfo.t.cpp
circle $FLAGS $PATHS bdlat_bdeatoverrides.t.cpp
circle $FLAGS $PATHS bdlat_choicefunctions.t.cpp
circle $FLAGS $PATHS bdlat_customizedtypefunctions.t.cpp
circle $FLAGS $PATHS bdlat_enumeratorinfo.t.cpp
circle $FLAGS $PATHS bdlat_enumfunctions.t.cpp
circle $FLAGS $PATHS bdlat_formattingmode.t.cpp
circle $FLAGS $PATHS bdlat_nullablevaluefunctions.t.cpp
circle $FLAGS $PATHS bdlat_selectioninfo.t.cpp
circle $FLAGS $PATHS bdlat_sequencefunctions.t.cpp
circle $FLAGS $PATHS bdlat_symbolicconverter.t.cpp
circle $FLAGS $PATHS bdlat_typecategory.t.cpp
circle $FLAGS $PATHS bdlat_typename.t.cpp
circle $FLAGS $PATHS bdlat_typetraits.t.cpp
circle $FLAGS $PATHS bdlat_valuetypefunctions.t.cpp