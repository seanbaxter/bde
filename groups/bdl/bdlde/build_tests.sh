set -x
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslma -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslh -I ../bdlb -I ../../bsl/bslstp -I ../../bsl/bslmt -I ../bdlf -I ../../bsl/bslx -I ../../bsl/bslim -I ../../bsl/bsltf"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bdlde_base64decoder.t.cpp
circle $PATHS $FLAGS bdlde_base64encoder.t.cpp
circle $PATHS $FLAGS bdlde_byteorder.t.cpp
circle $PATHS $FLAGS bdlde_charconvertstatus.t.cpp
circle $PATHS $FLAGS bdlde_charconvertucs2.t.cpp
circle $PATHS $FLAGS bdlde_charconvertutf16.t.cpp
circle $PATHS $FLAGS bdlde_charconvertutf32.t.cpp
circle $PATHS $FLAGS bdlde_crc32c.t.cpp
circle $PATHS $FLAGS bdlde_crc32.t.cpp
circle $PATHS $FLAGS bdlde_crc64.t.cpp
circle $PATHS $FLAGS bdlde_md5.t.cpp
circle $PATHS $FLAGS bdlde_quotedprintabledecoder.t.cpp
circle $PATHS $FLAGS bdlde_quotedprintableencoder.t.cpp
circle $PATHS $FLAGS bdlde_sha2.t.cpp
circle $PATHS $FLAGS bdlde_utf8util.t.cpp
