set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslalg -I ../bsl+bslhdrs -I ../bslstl -I ../bslmf -I ../bslh -I ../bsla -I ../bslma -I ../bsltf -I ../bslstp"
FLAGS="-filetype=o -O0 -stat"

circle $FLAGS $PATHS bslim_bslstandardheadertest.t.cpp
circle $FLAGS $PATHS bslim_gtestutil.t.cpp
circle $FLAGS $PATHS bslim_printer.t.cpp
circle $FLAGS $PATHS bslim_testutil.t.cpp
