set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslstl -I ../../bsl/bslh  -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslstp -I ../bdldfp -I ../../bsl/bslim -I ../bdlt -I ../../bsl/bslx -I ../../bsl/bsltf -I ../bdlma -I ../bdlsb -I ../../../thirdparty/inteldfp/LIBRARY/src"

circle $PATHS $FLAGS bdld_datumarraybuilder.t.cpp
circle $PATHS $FLAGS bdld_datumbinaryref.t.cpp
circle $PATHS $FLAGS bdld_datumerror.t.cpp
circle $PATHS $FLAGS bdld_datumintmapbuilder.t.cpp
circle $PATHS $FLAGS bdld_datummaker.t.cpp
circle $PATHS $FLAGS bdld_datummapbuilder.t.cpp
circle $PATHS $FLAGS bdld_datummapowningkeysbuilder.t.cpp
circle $PATHS $FLAGS bdld_datum.t.cpp
circle $PATHS $FLAGS bdld_datumudt.t.cpp
circle $PATHS $FLAGS bdld_manageddatum.t.cpp
