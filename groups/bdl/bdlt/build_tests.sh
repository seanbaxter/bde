set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslh -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../bdlb -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslim -I ../bdlc -I ../../bsl/bslstp -I ../bdlsb -I ../../bsl/bslmt -I ../../bsl/bsltf -I ../../bsl/bslx"

circle $PATHS $FLAGS bdlt_calendarcache.t.cpp
circle $PATHS $FLAGS bdlt_calendarloader.t.cpp
circle $PATHS $FLAGS bdlt_calendarreverseiteratoradapter.t.cpp
circle $PATHS $FLAGS bdlt_calendar.t.cpp
circle $PATHS $FLAGS bdlt_calendarutil.t.cpp
circle $PATHS $FLAGS bdlt_currenttime.t.cpp
circle $PATHS $FLAGS bdlt_date.t.cpp
circle $PATHS $FLAGS bdlt_datetimeimputil.t.cpp
circle $PATHS $FLAGS bdlt_datetimeinterval.t.cpp
circle $PATHS $FLAGS bdlt_datetimeintervalutil.t.cpp
circle $PATHS $FLAGS bdlt_datetime.t.cpp
circle $PATHS $FLAGS bdlt_datetimetz.t.cpp
circle $PATHS $FLAGS bdlt_datetimeutil.t.cpp
circle $PATHS $FLAGS bdlt_datetz.t.cpp
circle $PATHS $FLAGS bdlt_dateutil.t.cpp
circle $PATHS $FLAGS bdlt_dayofweekset.t.cpp
circle $PATHS $FLAGS bdlt_dayofweek.t.cpp
circle $PATHS $FLAGS bdlt_dayofweekutil.t.cpp
circle $PATHS $FLAGS bdlt_defaultcalendarcache.t.cpp
circle $PATHS $FLAGS bdlt_defaulttimetablecache.t.cpp
circle $PATHS $FLAGS bdlt_epochutil.t.cpp
circle $PATHS $FLAGS bdlt_fixutilconfiguration.t.cpp
circle $PATHS $FLAGS bdlt_fixutil.t.cpp
circle $PATHS $FLAGS bdlt_intervalconversionutil.t.cpp
circle $PATHS $FLAGS bdlt_iso8601utilconfiguration.t.cpp
circle $PATHS $FLAGS bdlt_iso8601util.t.cpp
circle $PATHS $FLAGS bdlt_localtimeoffset.t.cpp
circle $PATHS $FLAGS bdlt_monthofyear.t.cpp
circle $PATHS $FLAGS bdlt_packedcalendar.t.cpp
circle $PATHS $FLAGS bdlt_posixdateimputil.t.cpp
circle $PATHS $FLAGS bdlt_prolepticdateimputil.t.cpp
circle $PATHS $FLAGS bdlt_serialdateimputil.t.cpp
circle $PATHS $FLAGS bdlt_timetablecache.t.cpp
circle $PATHS $FLAGS bdlt_timetableloader.t.cpp
circle $PATHS $FLAGS bdlt_timetable.t.cpp
circle $PATHS $FLAGS bdlt_time.t.cpp
circle $PATHS $FLAGS bdlt_timetz.t.cpp
circle $PATHS $FLAGS bdlt_timeunitratio.t.cpp
circle $PATHS $FLAGS bdlt_timeutil.t.cpp
