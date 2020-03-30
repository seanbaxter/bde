set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslh -I ../../bsl/bslma -I ../../bsl/bslstp -I ../../bsl/bslalg -I ../../bsl/bslim -I ../bdlb -I ../../bsl/bslstl -I ../../../thirdparty/inteldfp/LIBRARY/src"

circle $PATHS $FLAGS bdldfp_decimalconvertutil.cpp
circle $PATHS $FLAGS bdldfp_decimalconvertutil_inteldfp.cpp
circle $PATHS $FLAGS bdldfp_decimal.cpp
circle $PATHS $FLAGS bdldfp_decimalformatconfig.cpp
circle $PATHS $FLAGS bdldfp_decimalimputil.cpp
circle $PATHS $FLAGS bdldfp_decimalimputil_inteldfp.cpp
circle $PATHS $FLAGS bdldfp_decimalplatform.cpp
circle $PATHS $FLAGS bdldfp_decimalstorage.cpp
circle $PATHS $FLAGS bdldfp_decimalutil.cpp
circle $PATHS $FLAGS bdldfp_intelimpwrapper.cpp
circle $PATHS $FLAGS bdldfp_uint128.cpp
