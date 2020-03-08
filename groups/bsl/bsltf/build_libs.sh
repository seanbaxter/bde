set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslma -I ../bslmf -I ../bsla"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bsltf_allocargumenttype.cpp
circle $PATHS $FLAGS bsltf_allocbitwisemoveabletesttype.cpp
circle $PATHS $FLAGS bsltf_allocemplacabletesttype.cpp
circle $PATHS $FLAGS bsltf_alloctesttype.cpp
circle $PATHS $FLAGS bsltf_argumenttype.cpp
circle $PATHS $FLAGS bsltf_bitwisecopyabletesttype.cpp
circle $PATHS $FLAGS bsltf_bitwisemoveabletesttype.cpp
circle $PATHS $FLAGS bsltf_convertiblevaluewrapper.cpp
circle $PATHS $FLAGS bsltf_degeneratefunctor.cpp
circle $PATHS $FLAGS bsltf_emplacabletesttype.cpp
circle $PATHS $FLAGS bsltf_enumeratedtesttype.cpp
circle $PATHS $FLAGS bsltf_evilbooleantype.cpp
circle $PATHS $FLAGS bsltf_movablealloctesttype.cpp
circle $PATHS $FLAGS bsltf_movabletesttype.cpp
circle $PATHS $FLAGS bsltf_moveonlyalloctesttype.cpp
circle $PATHS $FLAGS bsltf_movestate.cpp
circle $PATHS $FLAGS bsltf_nonassignabletesttype.cpp
circle $PATHS $FLAGS bsltf_noncopyconstructibletesttype.cpp
circle $PATHS $FLAGS bsltf_nondefaultconstructibletesttype.cpp
circle $PATHS $FLAGS bsltf_nonequalcomparabletesttype.cpp
circle $PATHS $FLAGS bsltf_nonoptionalalloctesttype.cpp
circle $PATHS $FLAGS bsltf_nontypicaloverloadstesttype.cpp
circle $PATHS $FLAGS bsltf_simpletesttype.cpp
circle $PATHS $FLAGS bsltf_stdallocatoradaptor.cpp
circle $PATHS $FLAGS bsltf_stdalloctesttype.cpp
circle $PATHS $FLAGS bsltf_stdstatefulallocator.cpp
circle $PATHS $FLAGS bsltf_stdtestallocator.cpp
circle $PATHS $FLAGS bsltf_streamutil.cpp
circle $PATHS $FLAGS bsltf_templatetestfacility.cpp
circle $PATHS $FLAGS bsltf_testvaluesarray.cpp
circle $PATHS $FLAGS bsltf_uniontesttype.cpp
circle $PATHS $FLAGS bsltf_wellbehavedmoveonlyalloctesttype.cpp
