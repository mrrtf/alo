#!/bin/zsh
#
# copy file from SAF data storage
#
# format of input file is :
#
# /alice/data/2015/LHC15o/000246757/pass1/15000246757019.100/AliESDs.FILTER_ESDMUON_WITH_ALIPHYSICS_v5-09-37-01-1.root
# /alice/data/2015/LHC15o/000246757/pass1/15000246757019.1000/AliESDs.FILTER_ESDMUON_WITH_ALIPHYSICS_v5-09-37-01-1.root


input=${1:-first100.txt}
user=aphecete

ssh -f4N -D 8000 ${user}@nansl1.in2p3.fr

module load AliEn-Runtime/latest

for f in $(cat ${input}); do
  fn=$(basename $f)
  dir=$(dirname $f)
  mkdir -p $dir
  cmd="xrdcp -D localhost:8000 root://nansafmaster2.in2p3.fr/$f $dir/$fn"
  echo $cmd
  eval $cmd
done

