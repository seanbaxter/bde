set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslma -I ../bslmf -I ../bsla"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bsltf_allocargumenttype.t.cpp
circle $PATHS $FLAGS bsltf_allocbitwisemoveabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_allocemplacabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_alloctesttype.t.cpp
circle $PATHS $FLAGS bsltf_argumenttype.t.cpp
circle $PATHS $FLAGS bsltf_bitwisecopyabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_bitwisemoveabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_convertiblevaluewrapper.t.cpp
circle $PATHS $FLAGS bsltf_degeneratefunctor.t.cpp
circle $PATHS $FLAGS bsltf_emplacabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_enumeratedtesttype.t.cpp
circle $PATHS $FLAGS bsltf_evilbooleantype.t.cpp
circle $PATHS $FLAGS bsltf_movablealloctesttype.t.cpp
circle $PATHS $FLAGS bsltf_movabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_moveonlyalloctesttype.t.cpp
circle $PATHS $FLAGS bsltf_movestate.t.cpp
circle $PATHS $FLAGS bsltf_nonassignabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_noncopyconstructibletesttype.t.cpp
circle $PATHS $FLAGS bsltf_nondefaultconstructibletesttype.t.cpp
circle $PATHS $FLAGS bsltf_nonequalcomparabletesttype.t.cpp
circle $PATHS $FLAGS bsltf_nonoptionalalloctesttype.t.cpp
circle $PATHS $FLAGS bsltf_nontypicaloverloadstesttype.t.cpp
circle $PATHS $FLAGS bsltf_simpletesttype.t.cpp
circle $PATHS $FLAGS bsltf_stdallocatoradaptor.t.cpp
circle $PATHS $FLAGS bsltf_stdalloctesttype.t.cpp
circle $PATHS $FLAGS bsltf_stdstatefulallocator.t.cpp
circle $PATHS $FLAGS bsltf_stdtestallocator.t.cpp
circle $PATHS $FLAGS bsltf_streamutil.t.cpp
circle $PATHS $FLAGS bsltf_templatetestfacility.t.cpp
circle $PATHS $FLAGS bsltf_testvaluesarray.t.cpp
circle $PATHS $FLAGS bsltf_uniontesttype.t.cpp
circle $PATHS $FLAGS bsltf_wellbehavedmoveonlyalloctesttype.t.cpp
