basedir=${1:-$(pwd)}

cmake ${basedir}/alo/r23/esdconvertor-saf/ \
	      -DCMAKE_INSTALL_PREFIX=${basedir} \
	      -DCMAKE_MODULE_PATH="${basedir}/alo/cmake;/${basedir}/O2/cmake/modules" \
	      -DO2_MCH_MAPPING_SRC="${basedir}/O2/Detectors/MUON/MCH/Mapping" \
	      -DALO_ESDCONVERTOR_SRC="${basedir}/alo/r23/esdconvertor" \
	      -DFLATBUFFERS_BINARY_DIR=$HOME/bin \
	      -DALIROOT=${ALICE_ROOT} \
	      -DROOT_DIR=${ROOTSYS}/cmake \
	      -DBoost_NO_SYSTEM_PATHS=ON \
	      -DBOOST_ROOT=${BOOST_ROOT} \
	      -DMS_GSL_INCLUDE_DIR=/cvmfs/alice-nightlies.cern.ch/el7-x86_64/Packages/ms_gsl/1-3/include/

nproc=$(getconf _NPROCESSORS_ONLN)
nproc=$((++nproc))

make -j${nproc} install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${basedir}/lib
export PATH=$PATH:${basedir}/bin

