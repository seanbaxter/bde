set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslma -I ../bslmf -I ../bsla -I ../bsltf"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslalg_arraydestructionprimitives.t.cpp
circle $PATHS $FLAGS bslalg_arrayprimitives.t.cpp
circle $PATHS $FLAGS bslalg_autoarraydestructor.t.cpp
circle $PATHS $FLAGS bslalg_autoarraymovedestructor.t.cpp
circle $PATHS $FLAGS bslalg_autoscalardestructor.t.cpp
circle $PATHS $FLAGS bslalg_bidirectionallinklistutil.t.cpp
circle $PATHS $FLAGS bslalg_bidirectionallink.t.cpp
circle $PATHS $FLAGS bslalg_bidirectionalnode.t.cpp
circle $PATHS $FLAGS bslalg_constructorproxy.t.cpp
circle $PATHS $FLAGS bslalg_containerbase.t.cpp
circle $PATHS $FLAGS bslalg_dequeimputil.t.cpp
circle $PATHS $FLAGS bslalg_dequeiterator.t.cpp
circle $PATHS $FLAGS bslalg_dequeprimitives.t.cpp
circle $PATHS $FLAGS bslalg_functoradapter.t.cpp
circle $PATHS $FLAGS bslalg_hashtableanchor.t.cpp
circle $PATHS $FLAGS bslalg_hashtablebucket.t.cpp
circle $PATHS $FLAGS bslalg_hashtableimputil.t.cpp
circle $PATHS $FLAGS bslalg_hashutil.t.cpp
circle $PATHS $FLAGS bslalg_hasstliterators.t.cpp
circle $PATHS $FLAGS bslalg_hastrait.t.cpp
circle $PATHS $FLAGS bslalg_rangecompare.t.cpp
circle $PATHS $FLAGS bslalg_rbtreeanchor.t.cpp
circle $PATHS $FLAGS bslalg_rbtreenode.t.cpp
circle $PATHS $FLAGS bslalg_rbtreeutil.t.cpp
circle $PATHS $FLAGS bslalg_scalardestructionprimitives.t.cpp
circle $PATHS $FLAGS bslalg_scalarprimitives.t.cpp
circle $PATHS $FLAGS bslalg_selecttrait.t.cpp
circle $PATHS $FLAGS bslalg_swaputil.t.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwisecopyable.t.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwiseequalitycomparable.t.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwisemoveable.t.cpp
circle $PATHS $FLAGS bslalg_typetraithaspointersemantics.t.cpp
circle $PATHS $FLAGS bslalg_typetraithasstliterators.t.cpp
circle $PATHS $FLAGS bslalg_typetraithastrivialdefaultconstructor.t.cpp
circle $PATHS $FLAGS bslalg_typetraitnil.t.cpp
circle $PATHS $FLAGS bslalg_typetraitpair.t.cpp
circle $PATHS $FLAGS bslalg_typetraits.t.cpp
circle $PATHS $FLAGS bslalg_typetraitusesbslmaallocator.t.cpp
