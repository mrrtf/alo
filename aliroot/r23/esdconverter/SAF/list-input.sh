#!/bin/bash

# split by chunks of n esd files
ninput=${1:-100}
basedir=${PWD}

for file in $(ls $basedir/*.list)
do
	n=0
	index=0

	f=$(basename $file)

	mkdir -p $basedir/$f.dir

	cd $basedir/$f.dir
	rm -rf input.*

	for l in $(cat $file)
	do
		echo root://nansafmaster2/$l >> input.$index
		(( n += 1))
		if (( $n % $ninput == 0 ))
		then
            echo "transfer_input_files=$basedir/$f.dir/input.$index,$basedir/bin/mch-convert-esd"
            cmd=$(cat input.$index | tr "\n" " ")
            echo "arguments = $index $cmd"
			echo "queue"
			(( index +=1 ))
			n=0
		fi
	done
done

if (( $n > 0 && $n < $ninput ))
then
            echo "transfer_input_files=$basedir/$f.dir/input.$index,$basedir/bin/mch-convert-esd"
            cmd=$(cat input.$index | tr "\n" " ")
            echo "arguments = $index $cmd"
			echo "queue"
fi
