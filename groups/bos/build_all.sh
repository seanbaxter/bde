set -x
cd bosscm;      sh build_all.sh; cd ..;
cd bos_stdhdrs; sh build_all.sh; cd ..;

# Fails bsl::map name lookup. What is this test trying to achieve?
# cd bostst;      sh build_all.sh; cd ..;
