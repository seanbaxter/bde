set -x
cd bal; sh build_all.sh;   cd ..;
cd bbl; sh build_all.sh;   cd ..;
cd bdl; sh build_all.sh;   cd ..;
cd bos; sh build_all.sh;   cd ..;
cd bsl; sh build_all.sh;   cd ..;

cd bal; sh build_tests.sh; cd ..;
cd bsl; sh build_tests.sh; cd ..;

