set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlb -I ../../bsl/bslma -I ../../bdl/bdlscm -I ../../bsl/bslim -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bslalg -I ../../bsl/bsla -I ../../bsl/bslx -I ../../bsl/bslstp -I ../../bdl/bdlsb -I ../../bsl/bslmt -I ../../bdl/bdlt -I ../../bdl/bdls -I ../../bdl/bdlma -I ../../bdl/bdlcc -I ../../bdl/bdlt -I ../../bdl/bdlf -I ../balb -I ../../bdl/bdlmt"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS ball_administration.cpp
circle $PATHS $FLAGS ball_asyncfileobserver.cpp
circle $PATHS $FLAGS ball_attributecontainer.cpp
circle $PATHS $FLAGS ball_attributecontainerlist.cpp
circle $PATHS $FLAGS ball_attributecontext.cpp
circle $PATHS $FLAGS ball_attribute.cpp
circle $PATHS $FLAGS ball_broadcastobserver.cpp
circle $PATHS $FLAGS ball_category.cpp
circle $PATHS $FLAGS ball_categorymanager.cpp
circle $PATHS $FLAGS ball_context.cpp
circle $PATHS $FLAGS ball_countingallocator.cpp
circle $PATHS $FLAGS ball_defaultattributecontainer.cpp
circle $PATHS $FLAGS ball_fileobserver2.cpp
circle $PATHS $FLAGS ball_fileobserver.cpp
circle $PATHS $FLAGS ball_filteringobserver.cpp
circle $PATHS $FLAGS ball_fixedsizerecordbuffer.cpp
circle $PATHS $FLAGS ball_log.cpp
circle $PATHS $FLAGS ball_logfilecleanerutil.cpp
circle $PATHS $FLAGS ball_loggercategoryutil.cpp
circle $PATHS $FLAGS ball_loggerfunctorpayloads.cpp
circle $PATHS $FLAGS ball_loggermanagerconfiguration.cpp
circle $PATHS $FLAGS ball_loggermanager.cpp
circle $PATHS $FLAGS ball_loggermanagerdefaults.cpp
circle $PATHS $FLAGS ball_logthrottle.cpp
circle $PATHS $FLAGS ball_multiplexobserver.cpp
circle $PATHS $FLAGS ball_observeradapter.cpp
circle $PATHS $FLAGS ball_observer.cpp
circle $PATHS $FLAGS ball_patternutil.cpp
circle $PATHS $FLAGS ball_predicate.cpp
circle $PATHS $FLAGS ball_predicateset.cpp
circle $PATHS $FLAGS ball_recordattributes.cpp
circle $PATHS $FLAGS ball_recordbuffer.cpp
circle $PATHS $FLAGS ball_record.cpp
circle $PATHS $FLAGS ball_recordstringformatter.cpp
circle $PATHS $FLAGS ball_rule.cpp
circle $PATHS $FLAGS ball_ruleset.cpp
circle $PATHS $FLAGS ball_scopedattribute.cpp
circle $PATHS $FLAGS ball_scopedattributes.cpp
circle $PATHS $FLAGS ball_severity.cpp
circle $PATHS $FLAGS ball_severityutil.cpp
circle $PATHS $FLAGS ball_streamobserver.cpp
circle $PATHS $FLAGS ball_testobserver.cpp
circle $PATHS $FLAGS ball_thresholdaggregate.cpp
circle $PATHS $FLAGS ball_transmission.cpp
circle $PATHS $FLAGS ball_userfields.cpp
circle $PATHS $FLAGS ball_userfieldtype.cpp
circle $PATHS $FLAGS ball_userfieldvalue.cpp
