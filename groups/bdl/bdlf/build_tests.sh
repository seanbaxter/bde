set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bsl/bslmf -I ../../bsl/bslma -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslim -I ../../bsl/bslstp -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bsltf"

circle $PATHS $FLAGS bdlf_bind.t.cpp
circle $PATHS $FLAGS bdlf_bind_test0.t.cpp
circle $PATHS $FLAGS bdlf_bind_test10.t.cpp
circle $PATHS $FLAGS bdlf_bind_test11.t.cpp
circle $PATHS $FLAGS bdlf_bind_test12.t.cpp
circle $PATHS $FLAGS bdlf_bind_test13.t.cpp
circle $PATHS $FLAGS bdlf_bind_test14.t.cpp
circle $PATHS $FLAGS bdlf_bind_test1.t.cpp
circle $PATHS $FLAGS bdlf_bind_test2.t.cpp
circle $PATHS $FLAGS bdlf_bind_test3.t.cpp
circle $PATHS $FLAGS bdlf_bind_test4.t.cpp
circle $PATHS $FLAGS bdlf_bind_test5.t.cpp
circle $PATHS $FLAGS bdlf_bind_test6.t.cpp
circle $PATHS $FLAGS bdlf_bind_test7.t.cpp
circle $PATHS $FLAGS bdlf_bind_test8.t.cpp
circle $PATHS $FLAGS bdlf_bind_test9.t.cpp
circle $PATHS $FLAGS bdlf_bind_testn.t.cpp
circle $PATHS $FLAGS bdlf_bind_test.t.cpp
circle $PATHS $FLAGS bdlf_memfn.t.cpp
circle $PATHS $FLAGS bdlf_placeholder.t.cpp
