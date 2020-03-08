set -x
PATHS="-I . -I ../bsl+bslhdrs -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla -I ../bslstl -I ../bslh -I ../bslalg -I ../bslma -I ../bsltf"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bslim_bslstandardheadertest.cpp
circle $PATHS $FLAGS bslim_gtestutil.cpp
circle $PATHS $FLAGS bslim_printer.cpp
circle $PATHS $FLAGS bslim_testutil.cpp
