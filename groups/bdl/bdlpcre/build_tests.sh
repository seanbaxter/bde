set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslalg -I ../../bsl/bslmt -I ../../bsl/bsltf -I ../bdlma -I ../../bsl/bslim
 -I ../../../thirdparty"

circle $FLAGS $PATHS bdlpcre_regex.t.cpp