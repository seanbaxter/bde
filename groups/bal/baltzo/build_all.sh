set -x

PATHS="-I . -I ../../bsl/bsls -I ../balscm/ -I ../../bsl/bslscm -I ../../bsl/bslalg -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslma -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslstp -I ../../bdl/bdlt -I ../../bsl/bslim -I ../../bdl/bdlscm -I ../../bdl/bdlb -I ../../bsl/bslx -I ../../bsl/bslmt -I ../../bdl/bdls -I ../../bdl/bdlsb"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS baltzo_datafileloader.cpp
circle $PATHS $FLAGS baltzo_defaultzoneinfocache.cpp
circle $PATHS $FLAGS baltzo_dstpolicy.cpp
circle $PATHS $FLAGS baltzo_errorcode.cpp
circle $PATHS $FLAGS baltzo_loader.cpp
circle $PATHS $FLAGS baltzo_localdatetime.cpp
circle $PATHS $FLAGS baltzo_localtimedescriptor.cpp
circle $PATHS $FLAGS baltzo_localtimeoffsetutil.cpp
circle $PATHS $FLAGS baltzo_localtimeperiod.cpp
circle $PATHS $FLAGS baltzo_localtimevalidity.cpp
circle $PATHS $FLAGS baltzo_testloader.cpp
circle $PATHS $FLAGS baltzo_timezoneutil.cpp
circle $PATHS $FLAGS baltzo_timezoneutilimp.cpp
circle $PATHS $FLAGS baltzo_windowstimezoneutil.cpp
circle $PATHS $FLAGS baltzo_zoneinfobinaryheader.cpp
circle $PATHS $FLAGS baltzo_zoneinfobinaryreader.cpp
circle $PATHS $FLAGS baltzo_zoneinfocache.cpp
circle $PATHS $FLAGS baltzo_zoneinfo.cpp
circle $PATHS $FLAGS baltzo_zoneinfoutil.cpp
