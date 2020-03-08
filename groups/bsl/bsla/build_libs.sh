set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bsla_annotations.cpp
circle $PATHS $FLAGS bsla_deprecated.cpp
circle $PATHS $FLAGS bsla_error.cpp
circle $PATHS $FLAGS bsla_fallthrough.cpp
circle $PATHS $FLAGS bsla_format.cpp
circle $PATHS $FLAGS bsla_maybeunused.cpp
circle $PATHS $FLAGS bsla_nodiscard.cpp
circle $PATHS $FLAGS bsla_nonnullarg.cpp
circle $PATHS $FLAGS bsla_noreturn.cpp
circle $PATHS $FLAGS bsla_nullterminated.cpp
circle $PATHS $FLAGS bsla_printf.cpp
circle $PATHS $FLAGS bsla_scanf.cpp
circle $PATHS $FLAGS bsla_unreachable.cpp
circle $PATHS $FLAGS bsla_unused.cpp
circle $PATHS $FLAGS bsla_used.cpp
circle $PATHS $FLAGS bsla_warning.cpp
