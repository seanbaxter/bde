set -x
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslma -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslh -I ../bdlb -I ../../bsl/bslstp -I ../../bsl/bslmt"
FLAGS="-filetype=o -O0 -stat"

circle $FLAGS $PATHS bdlde_base64decoder.cpp
circle $FLAGS $PATHS bdlde_base64encoder.cpp
circle $FLAGS $PATHS bdlde_byteorder.cpp
circle $FLAGS $PATHS bdlde_charconvertstatus.cpp
circle $FLAGS $PATHS bdlde_charconvertucs2.cpp
circle $FLAGS $PATHS bdlde_charconvertutf16.cpp
circle $FLAGS $PATHS bdlde_charconvertutf32.cpp
circle $FLAGS $PATHS bdlde_crc32c.cpp
circle $FLAGS $PATHS bdlde_crc32.cpp
circle $FLAGS $PATHS bdlde_crc64.cpp
circle $FLAGS $PATHS bdlde_md5.cpp
circle $FLAGS $PATHS bdlde_quotedprintabledecoder.cpp
circle $FLAGS $PATHS bdlde_quotedprintableencoder.cpp
circle $FLAGS $PATHS bdlde_sha2.cpp
circle $FLAGS $PATHS bdlde_utf8util.cpp
