PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bsl/bslx"
FLAGS="-filetype=o -O0 -stat"
set -x

circle $FLAGS $PATHS bdlb_algorithmworkaroundutil.cpp
circle $FLAGS $PATHS bdlb_arrayutil.cpp
circle $FLAGS $PATHS bdlb_bigendian.cpp
circle $FLAGS $PATHS bdlb_bitmaskutil.cpp
circle $FLAGS $PATHS bdlb_bitstringimputil.cpp
circle $FLAGS $PATHS bdlb_bitstringutil.cpp
circle $FLAGS $PATHS bdlb_bitutil.cpp
circle $FLAGS $PATHS bdlb_chartype.cpp
circle $FLAGS $PATHS bdlb_cstringequalto.cpp
circle $FLAGS $PATHS bdlb_cstringhash.cpp
circle $FLAGS $PATHS bdlb_cstringless.cpp
circle $FLAGS $PATHS bdlb_float.cpp
circle $FLAGS $PATHS bdlb_functionoutputiterator.cpp
circle $FLAGS $PATHS bdlb_guid.cpp
circle $FLAGS $PATHS bdlb_guidutil.cpp
circle $FLAGS $PATHS bdlb_hashutil.cpp
circle $FLAGS $PATHS bdlb_indexspan.cpp
circle $FLAGS $PATHS bdlb_indexspanstringutil.cpp
circle $FLAGS $PATHS bdlb_indexspanutil.cpp
circle $FLAGS $PATHS bdlb_literalutil.cpp
circle $FLAGS $PATHS bdlb_nullableallocatedvalue.cpp
circle $FLAGS $PATHS bdlb_nullablevalue.cpp
circle $FLAGS $PATHS bdlb_nullopt.cpp
circle $FLAGS $PATHS bdlb_nulloutputiterator.cpp
circle $FLAGS $PATHS bdlb_numericparseutil.cpp
circle $FLAGS $PATHS bdlb_print.cpp
circle $FLAGS $PATHS bdlb_printmethods.cpp
circle $FLAGS $PATHS bdlb_random.cpp
circle $FLAGS $PATHS bdlb_randomdevice.cpp
circle $FLAGS $PATHS bdlb_scopeexit.cpp
circle $FLAGS $PATHS bdlb_string.cpp
circle $FLAGS $PATHS bdlb_stringrefutil.cpp
circle $FLAGS $PATHS bdlb_testinputiterator.cpp
circle $FLAGS $PATHS bdlb_tokenizer.cpp
circle $FLAGS $PATHS bdlb_topologicalsortutil.cpp
circle $FLAGS $PATHS bdlb_transformiterator.cpp
circle $FLAGS $PATHS bdlb_variant.cpp