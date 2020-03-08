set -x
PATHS="-I . -I ../bsls -I ../bslscm -I ../bslmf -I ../bsla"
FLAGS="-O0 -filetype=o -stat"

circle $PATHS $FLAGS bsldoc_glossary.cpp