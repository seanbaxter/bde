set -x
PATHS="-I . -I ../bsls -I ../bslscm"
FLAGS="-filetype=o -O0 -stat"
circle $FLAGS $PATHS bsldoc_glossary.t.cpp
