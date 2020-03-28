PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslalg -I ../../bsl/bslstl -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bdl/bdlscm -I ../../bdl/bdlb -I ../../bsl/bslmt -I ../../bsl/bslim -I ../../bdl/bdlsb -I ../../bsl/bslx -I ../../bsl/bsltf -I ../../bdl/bdls"
FLAGS="-filetype=o -O0 -stat"
set -x

circle $PATHS $FLAGS baltzo_datafileloader.t.cpp
circle $PATHS $FLAGS baltzo_defaultzoneinfocache.t.cpp
circle $PATHS $FLAGS baltzo_dstpolicy.t.cpp
circle $PATHS $FLAGS baltzo_errorcode.t.cpp
circle $PATHS $FLAGS baltzo_loader.t.cpp
circle $PATHS $FLAGS baltzo_localdatetime.t.cpp
circle $PATHS $FLAGS baltzo_localtimedescriptor.t.cpp
circle $PATHS $FLAGS baltzo_localtimeoffsetutil.t.cpp
circle $PATHS $FLAGS baltzo_localtimeperiod.t.cpp
circle $PATHS $FLAGS baltzo_localtimevalidity.t.cpp
circle $PATHS $FLAGS baltzo_testloader.t.cpp
circle $PATHS $FLAGS baltzo_timezoneutilimp.t.cpp
circle $PATHS $FLAGS baltzo_timezoneutil.t.cpp
circle $PATHS $FLAGS baltzo_windowstimezoneutil.t.cpp
circle $PATHS $FLAGS baltzo_zoneinfobinaryheader.t.cpp
circle $PATHS $FLAGS baltzo_zoneinfobinaryreader.t.cpp
circle $PATHS $FLAGS baltzo_zoneinfocache.t.cpp
circle $PATHS $FLAGS baltzo_zoneinfo.t.cpp
circle $PATHS $FLAGS baltzo_zoneinfoutil.t.cpp
