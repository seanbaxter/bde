set -x
cd bblb;    sh build_libs.sh; cd ..;
cd bbldc;   sh build_libs.sh; cd ..;
cg bblscm;  sh build_libs.sh; cd ..;
