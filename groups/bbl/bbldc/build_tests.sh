set -x
PATHS="-I . -I ../../bsl/bsls -I ../bblscm -I ../../bsl/bslscm -I ../../bdl/bdlt -I ../../bsl/bsl+bslhdrs -I ../../bdl/bdlscm -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdlb -I ../../bsl/bslim -I ../../bdl/bdlc -I ../../bsl/bslstp -I ../../bsl/bsltf -I ../../bsl/bslx"
FLAGS="-filetype=o -O0 -stat"


circle $FLAGS $PATHS bbldc_basicactual360.t.cpp
circle $FLAGS $PATHS bbldc_basicactual365fixed.t.cpp
circle $FLAGS $PATHS bbldc_basicbasicdaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_basicdaterangedaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_basicdaycount.t.cpp
circle $FLAGS $PATHS bbldc_basicdaycountutil.t.cpp
circle $FLAGS $PATHS bbldc_basicisdaactualactual.t.cpp
circle $FLAGS $PATHS bbldc_basicisma30360.t.cpp
circle $FLAGS $PATHS bbldc_basicnl365.t.cpp
circle $FLAGS $PATHS bbldc_basicpsa30360eom.t.cpp
circle $FLAGS $PATHS bbldc_basicsia30360eom.t.cpp
circle $FLAGS $PATHS bbldc_basicsia30360neom.t.cpp
circle $FLAGS $PATHS bbldc_calendarbus252.t.cpp
circle $FLAGS $PATHS bbldc_calendardaterangedaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_calendardaycountutil.t.cpp
circle $FLAGS $PATHS bbldc_daterangedaycount.t.cpp
circle $FLAGS $PATHS bbldc_daycountconvention.t.cpp
circle $FLAGS $PATHS bbldc_perioddaterangedaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_perioddaycountutil.t.cpp
circle $FLAGS $PATHS bbldc_periodicmaactualactual.t.cpp
circle $FLAGS $PATHS bbldc_terminatedbasicdaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_terminateddaterangedaycountadapter.t.cpp
circle $FLAGS $PATHS bbldc_terminateddaycountutil.t.cpp
circle $FLAGS $PATHS bbldc_terminatedisda30360eom.t.cpp
