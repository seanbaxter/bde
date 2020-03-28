set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslx -I ../bdldfp -I ../../bsl/bslstp -I ../bdlt -I ../../../thirdparty/inteldfp/LIBRARY/src/"

circle $FLAGS $PATHS bdlat_arrayfunctions.cpp
circle $FLAGS $PATHS bdlat_arrayiterators.cpp
circle $FLAGS $PATHS bdlat_attributeinfo.cpp
circle $FLAGS $PATHS bdlat_bdeatoverrides.cpp
circle $FLAGS $PATHS bdlat_choicefunctions.cpp
circle $FLAGS $PATHS bdlat_customizedtypefunctions.cpp
circle $FLAGS $PATHS bdlat_enumeratorinfo.cpp
circle $FLAGS $PATHS bdlat_enumfunctions.cpp
circle $FLAGS $PATHS bdlat_formattingmode.cpp
circle $FLAGS $PATHS bdlat_nullablevaluefunctions.cpp
circle $FLAGS $PATHS bdlat_selectioninfo.cpp
circle $FLAGS $PATHS bdlat_sequencefunctions.cpp
circle $FLAGS $PATHS bdlat_symbolicconverter.cpp
circle $FLAGS $PATHS bdlat_typecategory.cpp
circle $FLAGS $PATHS bdlat_typename.cpp
circle $FLAGS $PATHS bdlat_typetraits.cpp
circle $FLAGS $PATHS bdlat_valuetypefunctions.cpp
