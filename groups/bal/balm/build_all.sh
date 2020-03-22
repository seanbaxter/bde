set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlt -I ../../bdl/bdlat -I ../../bdl/bdlscm -I ../../bsl/bslmf -I ../../bdl/bdlb -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslmt -I ../../bsl/bslx -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bdl/bdlf -I ../../bdl/bdlmt -I ../../bdl/bdlcc -I ../../bdl/bdlma"

circle $FLAGS $PATHS balm_category.cpp
circle $FLAGS $PATHS balm_collector.cpp
circle $FLAGS $PATHS balm_collectorrepository.cpp
circle $FLAGS $PATHS balm_configurationutil.cpp
circle $FLAGS $PATHS balm_defaultmetricsmanager.cpp
circle $FLAGS $PATHS balm_integercollector.cpp
circle $FLAGS $PATHS balm_integermetric.cpp
circle $FLAGS $PATHS balm_metric.cpp
circle $FLAGS $PATHS balm_metricdescription.cpp
circle $FLAGS $PATHS balm_metricformat.cpp
circle $FLAGS $PATHS balm_metricid.cpp
circle $FLAGS $PATHS balm_metricrecord.cpp
circle $FLAGS $PATHS balm_metricregistry.cpp
circle $FLAGS $PATHS balm_metricsample.cpp
circle $FLAGS $PATHS balm_metrics.cpp
circle $FLAGS $PATHS balm_metricsmanager.cpp
circle $FLAGS $PATHS balm_publicationscheduler.cpp
circle $FLAGS $PATHS balm_publicationtype.cpp
circle $FLAGS $PATHS balm_publisher.cpp
circle $FLAGS $PATHS balm_stopwatchscopedguard.cpp
circle $FLAGS $PATHS balm_streampublisher.cpp
