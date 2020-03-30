set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslh -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bsla -I ../bdlb -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslim -I ../bdlc -I ../../bsl/bslstp -I ../bdlsb -I ../../bsl/bslmt"

circle $FLAGS $PATHS bdlt_calendarcache.cpp
circle $FLAGS $PATHS bdlt_calendar.cpp
circle $FLAGS $PATHS bdlt_calendarloader.cpp
circle $FLAGS $PATHS bdlt_calendarreverseiteratoradapter.cpp
circle $FLAGS $PATHS bdlt_calendarutil.cpp
circle $FLAGS $PATHS bdlt_currenttime.cpp
circle $FLAGS $PATHS bdlt_date.cpp
circle $FLAGS $PATHS bdlt_datetime.cpp
circle $FLAGS $PATHS bdlt_datetimeimputil.cpp
circle $FLAGS $PATHS bdlt_datetimeinterval.cpp
circle $FLAGS $PATHS bdlt_datetimeintervalutil.cpp
circle $FLAGS $PATHS bdlt_datetimetz.cpp
circle $FLAGS $PATHS bdlt_datetimeutil.cpp
circle $FLAGS $PATHS bdlt_datetz.cpp
circle $FLAGS $PATHS bdlt_dateutil.cpp
circle $FLAGS $PATHS bdlt_dayofweek.cpp
circle $FLAGS $PATHS bdlt_dayofweekset.cpp
circle $FLAGS $PATHS bdlt_dayofweekutil.cpp
circle $FLAGS $PATHS bdlt_defaultcalendarcache.cpp
circle $FLAGS $PATHS bdlt_defaulttimetablecache.cpp
circle $FLAGS $PATHS bdlt_epochutil.cpp
circle $FLAGS $PATHS bdlt_fixutilconfiguration.cpp
circle $FLAGS $PATHS bdlt_fixutil.cpp
circle $FLAGS $PATHS bdlt_intervalconversionutil.cpp
circle $FLAGS $PATHS bdlt_iso8601utilconfiguration.cpp
circle $FLAGS $PATHS bdlt_iso8601util.cpp
circle $FLAGS $PATHS bdlt_localtimeoffset.cpp
circle $FLAGS $PATHS bdlt_monthofyear.cpp
circle $FLAGS $PATHS bdlt_packedcalendar.cpp
circle $FLAGS $PATHS bdlt_posixdateimputil.cpp
circle $FLAGS $PATHS bdlt_prolepticdateimputil.cpp
circle $FLAGS $PATHS bdlt_serialdateimputil.cpp
circle $FLAGS $PATHS bdlt_time.cpp
circle $FLAGS $PATHS bdlt_timetablecache.cpp
circle $FLAGS $PATHS bdlt_timetable.cpp
circle $FLAGS $PATHS bdlt_timetableloader.cpp
circle $FLAGS $PATHS bdlt_timetz.cpp
circle $FLAGS $PATHS bdlt_timeunitratio.cpp
circle $FLAGS $PATHS bdlt_timeutil.cpp
