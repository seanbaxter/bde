set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bslx -I ../../bsl/bslalg -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslmf -I ../../bsl/bslma -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslstl -I ../../bsl/bslstp -I ../../bsl/bslim"

circle $FLAGS $PATHS bdlc_bitarray.cpp
circle $FLAGS $PATHS bdlc_compactedarray.cpp
circle $FLAGS $PATHS bdlc_hashtable.cpp
circle $FLAGS $PATHS bdlc_indexclerk.cpp
circle $FLAGS $PATHS bdlc_packedintarray.cpp
circle $FLAGS $PATHS bdlc_packedintarrayutil.cpp
circle $FLAGS $PATHS bdlc_queue.cpp
