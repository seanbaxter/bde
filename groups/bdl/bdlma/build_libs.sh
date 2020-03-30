set -x
FLAGS="-I . -I ../bdlma -I ../../bsl/bsls -I ../bdlscm -I ../../bsl/bslscm -I ../../bsl/bslma -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bsl+bslhdrs -I ../bdlb -I ../../bsl/bslstl -I ../../bsl/bslh -I ../../bsl/bslstp -I ../../bsl/bslmt -I ../../bsl/bslalg"
PATHS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS bdlma_aligningallocator.cpp
circle $PATHS $FLAGS bdlma_autoreleaser.cpp
circle $PATHS $FLAGS bdlma_blocklist.cpp
circle $PATHS $FLAGS bdlma_bufferedsequentialallocator.cpp
circle $PATHS $FLAGS bdlma_bufferedsequentialpool.cpp
circle $PATHS $FLAGS bdlma_bufferimputil.cpp
circle $PATHS $FLAGS bdlma_buffermanager.cpp
circle $PATHS $FLAGS bdlma_concurrentallocatoradapter.cpp
circle $PATHS $FLAGS bdlma_concurrentfixedpool.cpp
circle $PATHS $FLAGS bdlma_concurrentmultipoolallocator.cpp
circle $PATHS $FLAGS bdlma_concurrentmultipool.cpp
circle $PATHS $FLAGS bdlma_concurrentpoolallocator.cpp
circle $PATHS $FLAGS bdlma_concurrentpool.cpp
circle $PATHS $FLAGS bdlma_countingallocator.cpp
circle $PATHS $FLAGS bdlma_defaultdeleter.cpp
circle $PATHS $FLAGS bdlma_deleter.cpp
circle $PATHS $FLAGS bdlma_factory.cpp
circle $PATHS $FLAGS bdlma_guardingallocator.cpp
circle $PATHS $FLAGS bdlma_heapbypassallocator.cpp
circle $PATHS $FLAGS bdlma_infrequentdeleteblocklist.cpp
circle $PATHS $FLAGS bdlma_localsequentialallocator.cpp
circle $PATHS $FLAGS bdlma_managedallocator.cpp
circle $PATHS $FLAGS bdlma_memoryblockdescriptor.cpp
circle $PATHS $FLAGS bdlma_multipoolallocator.cpp
circle $PATHS $FLAGS bdlma_multipool.cpp
circle $PATHS $FLAGS bdlma_pool.cpp
circle $PATHS $FLAGS bdlma_sequentialallocator.cpp
circle $PATHS $FLAGS bdlma_sequentialpool.cpp
