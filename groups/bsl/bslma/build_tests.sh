set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bslma_allocatoradaptor.t.cpp
circle $PATHS $FLAGS bslma_allocator.t.cpp
circle $PATHS $FLAGS bslma_allocatortraits.t.cpp
circle $PATHS $FLAGS bslma_autodeallocator.t.cpp
circle $PATHS $FLAGS bslma_autodestructor.t.cpp
circle $PATHS $FLAGS bslma_autorawdeleter.t.cpp
circle $PATHS $FLAGS bslma_bufferallocator.t.cpp
circle $PATHS $FLAGS bslma_constructionutil.t.cpp
circle $PATHS $FLAGS bslma_deallocatorguard.t.cpp
circle $PATHS $FLAGS bslma_deallocatorproctor.t.cpp
circle $PATHS $FLAGS bslma_defaultallocatorguard.t.cpp
circle $PATHS $FLAGS bslma_default.t.cpp
circle $PATHS $FLAGS bslma_deleterhelper.t.cpp
circle $PATHS $FLAGS bslma_destructionutil.t.cpp
circle $PATHS $FLAGS bslma_destructorguard.t.cpp
circle $PATHS $FLAGS bslma_destructorproctor.t.cpp
circle $PATHS $FLAGS bslma_exceptionguard.t.cpp
circle $PATHS $FLAGS bslma_infrequentdeleteblocklist.t.cpp
circle $PATHS $FLAGS bslma_mallocfreeallocator.t.cpp
circle $PATHS $FLAGS bslma_managedallocator.t.cpp
circle $PATHS $FLAGS bslma_managedptrdeleter.t.cpp
circle $PATHS $FLAGS bslma_managedptr_factorydeleter.t.cpp
circle $PATHS $FLAGS bslma_managedptr_members.t.cpp
circle $PATHS $FLAGS bslma_managedptr_pairproxy.t.cpp
circle $PATHS $FLAGS bslma_managedptr.t.cpp
circle $PATHS $FLAGS bslma_newdeleteallocator.t.cpp
circle $PATHS $FLAGS bslma_rawdeleterguard.t.cpp
circle $PATHS $FLAGS bslma_rawdeleterproctor.t.cpp
circle $PATHS $FLAGS bslma_sequentialallocator.t.cpp
circle $PATHS $FLAGS bslma_sequentialpool.t.cpp
circle $PATHS $FLAGS bslma_sharedptrinplacerep.t.cpp
circle $PATHS $FLAGS bslma_sharedptroutofplacerep.t.cpp
circle $PATHS $FLAGS bslma_sharedptrrep.t.cpp
circle $PATHS $FLAGS bslma_stdallocator.t.cpp
circle $PATHS $FLAGS bslma_stdtestallocator.t.cpp
circle $PATHS $FLAGS bslma_testallocatorexception.t.cpp
circle $PATHS $FLAGS bslma_testallocatormonitor.t.cpp
circle $PATHS $FLAGS bslma_testallocator.t.cpp
circle $PATHS $FLAGS bslma_usesbslmaallocator.t.cpp
