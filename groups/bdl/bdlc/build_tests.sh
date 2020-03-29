set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bslx -I ../../bsl/bslalg -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslmf -I ../../bsl/bslma -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslstl -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bsl/bsltf -I ../bdlma"

circle $FLAGS $PATHS bdlc_bitarray.t.cpp
circle $FLAGS $PATHS bdlc_compactedarray.t.cpp
circle $FLAGS $PATHS bdlc_hashtable.t.cpp
circle $FLAGS $PATHS bdlc_indexclerk.t.cpp
circle $FLAGS $PATHS bdlc_packedintarray.t.cpp
circle $FLAGS $PATHS bdlc_packedintarrayutil.t.cpp
circle $FLAGS $PATHS bdlc_queue.t.cpp
