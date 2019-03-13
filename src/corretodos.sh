#!/bin/bash

cores=('8' '16' '32')
binaries=('Validation_ForNoWait');
samples='5'

rm results.txt
for binary in ${binaries[@]}; do
	echo "$binary" | tee -a results.txt
	for core in ${cores[@]}; do
		echo "$core" | tee -a results.txt
		for ((i=0;i<samples;i++)); do
			times=$(srun -p roquer time ./$name 300000000 $core 2>&1 | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
			echo "$result" | tee -a results.txt	
		done
	done
done


