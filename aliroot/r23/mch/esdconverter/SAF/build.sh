basedir=${1:-$(pwd)}

mkdir -p ${basedir}/build/alo
pushd ${basedir}/build/alo

$HOME/bin/cmake ${basedir}/alo \
	      -DCMAKE_INSTALL_PREFIX=${basedir} \
	      -DCMAKE_MODULE_PATH="${basedir}/alo/cmake" \
	      -DALIROOT="${ALICE_ROOT}" \
              -DBOOST_ROOT="$BOOST_ROOT"


nproc=$(getconf _NPROCESSORS_ONLN)
nproc=$((++nproc))

make -j${nproc} install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${basedir}/lib
export PATH=$PATH:${basedir}/bin

popd

