
#!/bin/bash
rm results.txt
cores=('2' '4' '8' '16' '32')
binari=('QS_Paralel' 'QS_ParalelNested' 'Validation_For' 'Validation_ForNoWait' 'Validation_Sections');
for name in ${binari[@]}; do

	echo "$name" >> results.txt
	echo "$name"
	i="0"
	for core in ${cores[@]}; do
		while [ $i -lt 10 ]
		do
			echo "srun -p gat time ./$name 300000000 $core 2>&1"
			times=$(srun -p gat time ./$name 300000000 $core 2>&1)
			result=$(echo "$time" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
			echo "result is $result"
			echo "$times"
			echo "$times " >> results.txt	
			i=$[$i+1]
		done
	done
done


