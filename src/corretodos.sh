#!/bin/bash

cores=('4' '8' '16' '32')
binaries=('P2.1_CPM_MarineRuiz');
servers=("gat" "roquer" "teen")
samples='10'

rm results.txt
for binary in ${binaries[@]}; do
	echo "$binary" | tee -a results.txt
	for server in ${servers[@]}; do
		echo "$server" | tee -a results.txt
		for core in ${cores[@]}; do
			echo "$core" | tee -a results.txt
			for ((i=0;i<samples;i++)); do
				times=$(srun -p $server time ./$binary $core 2>&1 | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
				echo "$times" | tee -a results.txt	
			done
		done
	done
done

