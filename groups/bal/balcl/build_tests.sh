set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstl -I ../../bsl/bslstp -I ../../bsl/bslh -I ../../bsl/bslmf -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdlt -I ../../bdl/bdlb -I ../../bdl/bdlscm -I ../../bsl/bslx -I ../../bsl/bslim -I ../../bsl/bsltf"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS balcl_commandline.t.cpp
circle $PATHS $FLAGS balcl_constraint.t.cpp
circle $PATHS $FLAGS balcl_occurrenceinfo.t.cpp
circle $PATHS $FLAGS balcl_optioninfo.t.cpp
circle $PATHS $FLAGS balcl_option.t.cpp
circle $PATHS $FLAGS balcl_optiontype.t.cpp
circle $PATHS $FLAGS balcl_optionvalue.t.cpp
circle $PATHS $FLAGS balcl_typeinfo.t.cpp
