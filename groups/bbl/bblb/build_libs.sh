set -x
PATHS="-I . -I ../../bsl/bsls -I ../bblb -I ../bblscm -I ../../bsl/bslscm -I ../../bdl/bdlt -I ../../bdl/bdlscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslh -I ../../bdl/bdlb -I ../../bdl/bdlc -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bslstp"
FLAGS="-filetype=o -O0 -stat"
circle $FLAGS $PATHS bblb_schedulegenerationutil.cpp