set -x
PATHS="-I . -I ../../bsl/bsls -I ../bblscm -I ../../bsl/bslscm -I ../../bdl/bdlt -I ../../bsl/bsl+bslhdrs -I ../../bdl/bdlscm -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdlb -I ../../bsl/bslim -I ../../bdl/bdlc -I ../../bsl/bslstp"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bbldc_basicactual360.cpp
circle $PATHS $FLAGS bbldc_basicactual365fixed.cpp
circle $PATHS $FLAGS bbldc_basicbasicdaycountadapter.cpp
circle $PATHS $FLAGS bbldc_basicdaterangedaycountadapter.cpp
circle $PATHS $FLAGS bbldc_basicdaycount.cpp
circle $PATHS $FLAGS bbldc_basicdaycountutil.cpp
circle $PATHS $FLAGS bbldc_basicisdaactualactual.cpp
circle $PATHS $FLAGS bbldc_basicisma30360.cpp
circle $PATHS $FLAGS bbldc_basicnl365.cpp
circle $PATHS $FLAGS bbldc_basicpsa30360eom.cpp
circle $PATHS $FLAGS bbldc_basicsia30360eom.cpp
circle $PATHS $FLAGS bbldc_basicsia30360neom.cpp
circle $PATHS $FLAGS bbldc_calendarbus252.cpp
circle $PATHS $FLAGS bbldc_calendardaterangedaycountadapter.cpp
circle $PATHS $FLAGS bbldc_calendardaycountutil.cpp
circle $PATHS $FLAGS bbldc_daterangedaycount.cpp
circle $PATHS $FLAGS bbldc_daycountconvention.cpp
circle $PATHS $FLAGS bbldc_perioddaterangedaycountadapter.cpp
circle $PATHS $FLAGS bbldc_perioddaycountutil.cpp
circle $PATHS $FLAGS bbldc_periodicmaactualactual.cpp
circle $PATHS $FLAGS bbldc_terminatedbasicdaycountadapter.cpp
circle $PATHS $FLAGS bbldc_terminateddaterangedaycountadapter.cpp
circle $PATHS $FLAGS bbldc_terminateddaycountutil.cpp
circle $PATHS $FLAGS bbldc_terminatedisda30360eom.cpp
