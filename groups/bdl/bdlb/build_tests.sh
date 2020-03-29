PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bsl/bslx -I ../../bsl/bsltf"
FLAGS="-filetype=o -O0 -stat"
set -x

circle $FLAGS $PATHS bdlb_algorithmworkaroundutil.t.cpp
circle $FLAGS $PATHS bdlb_arrayutil.t.cpp
circle $FLAGS $PATHS bdlb_bigendian.t.cpp
circle $FLAGS $PATHS bdlb_bitmaskutil.t.cpp
circle $FLAGS $PATHS bdlb_bitstringimputil.t.cpp
circle $FLAGS $PATHS bdlb_bitstringutil.t.cpp
circle $FLAGS $PATHS bdlb_bitutil.t.cpp
circle $FLAGS $PATHS bdlb_chartype.t.cpp
circle $FLAGS $PATHS bdlb_cstringequalto.t.cpp
circle $FLAGS $PATHS bdlb_cstringhash.t.cpp
circle $FLAGS $PATHS bdlb_cstringless.t.cpp
circle $FLAGS $PATHS bdlb_float.t.cpp
circle $FLAGS $PATHS bdlb_functionoutputiterator.t.cpp
circle $FLAGS $PATHS bdlb_guid.t.cpp
circle $FLAGS $PATHS bdlb_guidutil.t.cpp
circle $FLAGS $PATHS bdlb_hashutil.t.cpp
circle $FLAGS $PATHS bdlb_indexspanstringutil.t.cpp
circle $FLAGS $PATHS bdlb_indexspan.t.cpp
circle $FLAGS $PATHS bdlb_indexspanutil.t.cpp
circle $FLAGS $PATHS bdlb_literalutil.t.cpp
circle $FLAGS $PATHS bdlb_nullableallocatedvalue.t.cpp
circle $FLAGS $PATHS bdlb_nullablevalue.t.cpp
circle $FLAGS $PATHS bdlb_nullopt.t.cpp
circle $FLAGS $PATHS bdlb_nulloutputiterator.t.cpp
circle $FLAGS $PATHS bdlb_numericparseutil.t.cpp
circle $FLAGS $PATHS bdlb_printmethods.t.cpp
circle $FLAGS $PATHS bdlb_print.t.cpp
circle $FLAGS $PATHS bdlb_randomdevice.t.cpp
circle $FLAGS $PATHS bdlb_random.t.cpp
circle $FLAGS $PATHS bdlb_scopeexit.t.cpp
circle $FLAGS $PATHS bdlb_stringrefutil.t.cpp
circle $FLAGS $PATHS bdlb_string.t.cpp
circle $FLAGS $PATHS bdlb_testinputiterator.t.cpp
circle $FLAGS $PATHS bdlb_tokenizer.t.cpp
circle $FLAGS $PATHS bdlb_topologicalsortutil.t.cpp
circle $FLAGS $PATHS bdlb_transformiterator.t.cpp
circle $FLAGS $PATHS bdlb_variant.t.cpp
