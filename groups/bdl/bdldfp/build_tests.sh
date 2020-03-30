set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslh -I ../../bsl/bslma -I ../../bsl/bslstp -I ../../bsl/bslalg -I ../../bsl/bslim -I ../bdlb -I ../../bsl/bslstl -I ../../../thirdparty/inteldfp/LIBRARY/src -I ../../bsl/bsltf -I ../bdlsb -I ../../bsl/bslx -I ../../bsl/bslmt"

circle $PATHS $FLAGS bdldfp_decimalconvertutil_inteldfp.t.cpp
circle $PATHS $FLAGS bdldfp_decimalconvertutil.t.cpp
circle $PATHS $FLAGS bdldfp_decimalformatconfig.t.cpp
circle $PATHS $FLAGS bdldfp_decimalimputil_inteldfp.t.cpp
circle $PATHS $FLAGS bdldfp_decimalimputil.t.cpp
circle $PATHS $FLAGS bdldfp_decimalplatform.t.cpp
circle $PATHS $FLAGS bdldfp_decimalstorage.t.cpp
circle $PATHS $FLAGS bdldfp_decimal.t.cpp
circle $PATHS $FLAGS bdldfp_decimalutil.t.cpp
circle $PATHS $FLAGS bdldfp_intelimpwrapper.t.cpp
circle $PATHS $FLAGS bdldfp_uint128.t.cpp