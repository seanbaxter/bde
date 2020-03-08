set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle bslma_allocatoradaptor.cpp              $PATHS $FLAGS
circle bslma_allocator.cpp                     $PATHS $FLAGS
circle bslma_allocatortraits.cpp               $PATHS $FLAGS
circle bslma_autodeallocator.cpp               $PATHS $FLAGS
circle bslma_autodestructor.cpp                $PATHS $FLAGS
circle bslma_autorawdeleter.cpp                $PATHS $FLAGS
circle bslma_bufferallocator.cpp               $PATHS $FLAGS
circle bslma_constructionutil.cpp              $PATHS $FLAGS
circle bslma_deallocatorguard.cpp              $PATHS $FLAGS
circle bslma_deallocatorproctor.cpp            $PATHS $FLAGS
circle bslma_defaultallocatorguard.cpp         $PATHS $FLAGS
circle bslma_default.cpp                       $PATHS $FLAGS
circle bslma_deleterhelper.cpp                 $PATHS $FLAGS
circle bslma_destructionutil.cpp               $PATHS $FLAGS
circle bslma_destructorguard.cpp               $PATHS $FLAGS
circle bslma_destructorproctor.cpp             $PATHS $FLAGS
circle bslma_exceptionguard.cpp                $PATHS $FLAGS
circle bslma_infrequentdeleteblocklist.cpp     $PATHS $FLAGS
circle bslma_mallocfreeallocator.cpp           $PATHS $FLAGS
circle bslma_managedallocator.cpp              $PATHS $FLAGS
circle bslma_managedptr.cpp                    $PATHS $FLAGS
circle bslma_managedptrdeleter.cpp             $PATHS $FLAGS
circle bslma_managedptr_factorydeleter.cpp     $PATHS $FLAGS
circle bslma_managedptr_members.cpp            $PATHS $FLAGS
circle bslma_managedptr_pairproxy.cpp          $PATHS $FLAGS
circle bslma_newdeleteallocator.cpp            $PATHS $FLAGS
circle bslma_rawdeleterguard.cpp               $PATHS $FLAGS
circle bslma_rawdeleterproctor.cpp             $PATHS $FLAGS
circle bslma_sequentialallocator.cpp           $PATHS $FLAGS
circle bslma_sequentialpool.cpp                $PATHS $FLAGS
circle bslma_sharedptrinplacerep.cpp           $PATHS $FLAGS
circle bslma_sharedptroutofplacerep.cpp        $PATHS $FLAGS
circle bslma_sharedptrrep.cpp                  $PATHS $FLAGS
circle bslma_stdallocator.cpp                  $PATHS $FLAGS
circle bslma_stdtestallocator.cpp              $PATHS $FLAGS
circle bslma_testallocator.cpp                 $PATHS $FLAGS
circle bslma_testallocatorexception.cpp        $PATHS $FLAGS
circle bslma_testallocatormonitor.cpp          $PATHS $FLAGS
circle bslma_usesbslmaallocator.cpp            $PATHS $FLAGS
