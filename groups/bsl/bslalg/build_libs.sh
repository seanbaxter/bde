set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslma -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bslalg_arraydestructionprimitives.cpp
circle $PATHS $FLAGS bslalg_arrayprimitives.cpp
circle $PATHS $FLAGS bslalg_autoarraydestructor.cpp
circle $PATHS $FLAGS bslalg_autoarraymovedestructor.cpp
circle $PATHS $FLAGS bslalg_autoscalardestructor.cpp
circle $PATHS $FLAGS bslalg_bidirectionallink.cpp
circle $PATHS $FLAGS bslalg_bidirectionallinklistutil.cpp
circle $PATHS $FLAGS bslalg_bidirectionalnode.cpp
circle $PATHS $FLAGS bslalg_constructorproxy.cpp
circle $PATHS $FLAGS bslalg_containerbase.cpp
circle $PATHS $FLAGS bslalg_dequeimputil.cpp
circle $PATHS $FLAGS bslalg_dequeiterator.cpp
circle $PATHS $FLAGS bslalg_dequeprimitives.cpp
circle $PATHS $FLAGS bslalg_functoradapter.cpp
circle $PATHS $FLAGS bslalg_hashtableanchor.cpp
circle $PATHS $FLAGS bslalg_hashtablebucket.cpp
circle $PATHS $FLAGS bslalg_hashtableimputil.cpp
circle $PATHS $FLAGS bslalg_hashutil.cpp
circle $PATHS $FLAGS bslalg_hasstliterators.cpp
circle $PATHS $FLAGS bslalg_hastrait.cpp
circle $PATHS $FLAGS bslalg_rangecompare.cpp
circle $PATHS $FLAGS bslalg_rbtreeanchor.cpp
circle $PATHS $FLAGS bslalg_rbtreenode.cpp
circle $PATHS $FLAGS bslalg_rbtreeutil.cpp
circle $PATHS $FLAGS bslalg_scalardestructionprimitives.cpp
circle $PATHS $FLAGS bslalg_scalarprimitives.cpp
circle $PATHS $FLAGS bslalg_selecttrait.cpp
circle $PATHS $FLAGS bslalg_swaputil.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwisecopyable.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwisecopyable.t.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwiseequalitycomparable.cpp
circle $PATHS $FLAGS bslalg_typetraitbitwisemoveable.cpp
circle $PATHS $FLAGS bslalg_typetraithaspointersemantics.cpp
circle $PATHS $FLAGS bslalg_typetraithasstliterators.cpp
circle $PATHS $FLAGS bslalg_typetraithastrivialdefaultconstructor.cpp
circle $PATHS $FLAGS bslalg_typetraitnil.cpp
circle $PATHS $FLAGS bslalg_typetraitpair.cpp
circle $PATHS $FLAGS bslalg_typetraits.cpp
circle $PATHS $FLAGS bslalg_typetraitusesbslmaallocator.cpp
