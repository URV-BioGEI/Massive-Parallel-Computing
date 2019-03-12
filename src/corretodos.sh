
#!/bin/bash
rm results.txt
cores=('2' '4' '8' '16' '32')
binari=('Validation_ForNoWait');
for name in ${binari[@]}; do

	echo "$name" >> results.txt
	echo "$name"
	for core in ${cores[@]}; do
		echo "$core" >> results.txt
		i="0"
		while [ $i -lt 5 ]
		do
			echo "$i"
			echo "srun -p gat time ./$name 300000000 $core 2>&1"
			times=$(srun -p gat time ./$name 300000000 $core 2>&1)
			result=$(echo "$times" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
			echo "result is $result"
			echo "$times"
			echo "$result" >> results.txt	
			i=$[$i+1]
		done
	done
done


