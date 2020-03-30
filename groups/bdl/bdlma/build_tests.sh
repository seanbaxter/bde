set -x
FLAGS="-I . -I ../bdlma -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../bdlb -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../../bsl/bslmt -I ../../bsl/bslalg -I ../../bsl/bslim -I ../../bsl/bsltf -I ../bdlf"
PATHS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bdlma_alignedallocator.t.cpp
circle $PATHS $FLAGS bdlma_aligningallocator.t.cpp
circle $PATHS $FLAGS bdlma_autoreleaser.t.cpp
circle $PATHS $FLAGS bdlma_blocklist.t.cpp
circle $PATHS $FLAGS bdlma_bufferedsequentialallocator.t.cpp
circle $PATHS $FLAGS bdlma_bufferedsequentialpool.t.cpp
circle $PATHS $FLAGS bdlma_bufferimputil.t.cpp
circle $PATHS $FLAGS bdlma_buffermanager.t.cpp
circle $PATHS $FLAGS bdlma_concurrentallocatoradapter.t.cpp
circle $PATHS $FLAGS bdlma_concurrentfixedpool.t.cpp
circle $PATHS $FLAGS bdlma_concurrentmultipoolallocator.t.cpp
circle $PATHS $FLAGS bdlma_concurrentmultipool.t.cpp
circle $PATHS $FLAGS bdlma_concurrentpoolallocator.t.cpp
circle $PATHS $FLAGS bdlma_concurrentpool.t.cpp
circle $PATHS $FLAGS bdlma_countingallocator.t.cpp
circle $PATHS $FLAGS bdlma_defaultdeleter.t.cpp
circle $PATHS $FLAGS bdlma_deleter.t.cpp
circle $PATHS $FLAGS bdlma_factory.t.cpp
circle $PATHS $FLAGS bdlma_guardingallocator.t.cpp
circle $PATHS $FLAGS bdlma_heapbypassallocator.t.cpp
circle $PATHS $FLAGS bdlma_infrequentdeleteblocklist.t.cpp
circle $PATHS $FLAGS bdlma_localsequentialallocator.t.cpp
circle $PATHS $FLAGS bdlma_managedallocator.t.cpp
circle $PATHS $FLAGS bdlma_memoryblockdescriptor.t.cpp
circle $PATHS $FLAGS bdlma_multipoolallocator.t.cpp
circle $PATHS $FLAGS bdlma_multipool.t.cpp
circle $PATHS $FLAGS bdlma_pool.t.cpp
circle $PATHS $FLAGS bdlma_sequentialallocator.t.cpp
circle $PATHS $FLAGS bdlma_sequentialpool.t.cpp
