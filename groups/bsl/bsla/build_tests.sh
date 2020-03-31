set -x

FLAGS="-filetype=o -O0 -fPIC -stat"
PATHS="-I . -I ../bsls"

circle $FLAGS $PATHS bsla_annotations.t.cpp
circle $FLAGS $PATHS bsla_deprecated.t.cpp
circle $FLAGS $PATHS bsla_error.t.cpp
circle $FLAGS $PATHS bsla_fallthrough.t.cpp
circle $FLAGS $PATHS bsla_format.t.cpp
circle $FLAGS $PATHS bsla_maybeunused.t.cpp
circle $FLAGS $PATHS bsla_nodiscard.t.cpp
circle $FLAGS $PATHS bsla_nonnullarg.t.cpp
circle $FLAGS $PATHS bsla_noreturn.t.cpp
circle $FLAGS $PATHS bsla_nullterminated.t.cpp
circle $FLAGS $PATHS bsla_printf.t.cpp
circle $FLAGS $PATHS bsla_scanf.t.cpp
circle $FLAGS $PATHS bsla_unreachable.t.cpp
circle $FLAGS $PATHS bsla_unused.t.cpp
circle $FLAGS $PATHS bsla_used.t.cpp
circle $FLAGS $PATHS bsla_warning.t.cpp