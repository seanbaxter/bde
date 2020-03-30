set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../bdlb -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslmf -I ../../bsl/bslstl -I ../../bsl/bslh  -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslstp -I ../bdldfp -I ../../bsl/bslim -I ../bdlt -I ../../bsl/bslx -I ../../../thirdparty/inteldfp/LIBRARY/src"

circle $PATHS $FLAGS bdld_datumarraybuilder.cpp
circle $PATHS $FLAGS bdld_datumbinaryref.cpp
circle $PATHS $FLAGS bdld_datum.cpp
circle $PATHS $FLAGS bdld_datumerror.cpp
circle $PATHS $FLAGS bdld_datumintmapbuilder.cpp
circle $PATHS $FLAGS bdld_datummaker.cpp
circle $PATHS $FLAGS bdld_datummapbuilder.cpp
circle $PATHS $FLAGS bdld_datummapowningkeysbuilder.cpp
circle $PATHS $FLAGS bdld_datumudt.cpp
circle $PATHS $FLAGS bdld_manageddatum.cpp
