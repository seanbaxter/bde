set -x
PATHS="-I . -I ../../bsl/bsls -I ../balscm -I ../../bsl/bslscm -I ../../bsl/bsl+bslhdrs -I ../../bsl/bslstp -I ../../bsl/bslstl -I ../../bsl/bslmf -I ../../bsl/bslh -I ../../bsl/bsla -I ../../bsl/bslalg -I ../../bsl/bslma -I ../../bdl/bdlt -I ../../bdl/bdlb -I ../../bdl/bdlscm -I ../../bsl/bslx -I ../../bsl/bslim"
FLAGS="-filetype=o -O0 -stat"

circle $PATHS $FLAGS balcl_commandline.cpp
circle $PATHS $FLAGS balcl_constraint.cpp
circle $PATHS $FLAGS balcl_occurrenceinfo.cpp
circle $PATHS $FLAGS balcl_option.cpp
circle $PATHS $FLAGS balcl_optioninfo.cpp
circle $PATHS $FLAGS balcl_optiontype.cpp
circle $PATHS $FLAGS balcl_optionvalue.cpp
circle $PATHS $FLAGS balcl_typeinfo.cpp
