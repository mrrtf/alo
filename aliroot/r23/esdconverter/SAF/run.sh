#!/bin/bash

source /cvmfs/alice.cern.ch/etc/login.sh

source $(which alienv) setenv VO_ALICE@AliRoot::v5-09-37c_ROOT6-1 

alienv list

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)
export PATH=${PATH}:$(pwd)

basename=$1
shift

echo "basename=$basename"

./mch-convert-esd-to-digits --detelemids 100 --basename MCH-DIGITS-$basename- $@

rm -f tmp.*

ls -alR 

