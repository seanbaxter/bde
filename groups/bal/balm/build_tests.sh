set -x
FLAGS="-filetype=o -O0 -stat"
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bdl/bdlat -I ../../bdl/bdlscm -I ../../bdl/bdlb -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslma -I ../../bsl/bslalg -I ../../bsl/bslmt -I ../../bsl/bslstp -I ../../bsl/bslim -I ../../bdl/bdlt -I ../../bsl/bsltf -I ../../bdl/bdlmt -I ../../bsl/bslx -I ../../bdl/bdlcc -I ../../bdl/bdlf -I ../../bdl/bdlma"

circle $FLAGS $PATHS balm_category.t.cpp
circle $FLAGS $PATHS balm_collectorrepository.t.cpp
circle $FLAGS $PATHS balm_collector.t.cpp
circle $FLAGS $PATHS balm_configurationutil.t.cpp
circle $FLAGS $PATHS balm_defaultmetricsmanager.t.cpp
circle $FLAGS $PATHS balm_integercollector.t.cpp
circle $FLAGS $PATHS balm_integermetric.t.cpp
circle $FLAGS $PATHS balm_metricdescription.t.cpp
circle $FLAGS $PATHS balm_metricformat.t.cpp
circle $FLAGS $PATHS balm_metricid.t.cpp
circle $FLAGS $PATHS balm_metricrecord.t.cpp
circle $FLAGS $PATHS balm_metricregistry.t.cpp
circle $FLAGS $PATHS balm_metricsample.t.cpp
circle $FLAGS $PATHS balm_metricsmanager.t.cpp
circle $FLAGS $PATHS balm_metrics.t.cpp
circle $FLAGS $PATHS balm_metric.t.cpp
circle $FLAGS $PATHS balm_publicationscheduler.t.cpp
circle $FLAGS $PATHS balm_publicationtype.t.cpp
circle $FLAGS $PATHS balm_publisher.t.cpp
circle $FLAGS $PATHS balm_stopwatchscopedguard.t.cpp
circle $FLAGS $PATHS balm_streampublisher.t.cpp
