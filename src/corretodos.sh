#!/bin/bash
rm results.txt
binari=('QS_Paralel' 'QS_ParalelNested' 'Validation_For' 'Validation_ForNoWait' 'Validation_Sections');
for name in ${binari[@]}; do

	echo "$name" >> results.txt
	echo "$name"
	i="0"
	while [ $i -lt 10 ]
	do
		time=$(srun -p gat time ./$name 300000000 32 2>&1 | grep "real" | cut -d ' ' -f3 | cut -c 3-7)
		echo "$time"
		echo "$time " >> results.txt	
		i=$[$i+1]
	done
done


