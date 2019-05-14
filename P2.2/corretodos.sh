
#!/bin/bash
rm results.txt
nodes=('2' '2' '4' '4' '8' '8' '8' )
procesos=('2' '4' '4' '8' '8' '16' '32')
binari=('P2.2_CPM_MarineRuiz');
for name in ${binari[@]}; do
	echo "$name" >> results.txt
	echo "$name"
	contproc=0
	for core in ${nodes[@]}; do
		proc=${procesos[$contproc]}
		echo "$core $proc" >> results.txt
		i="0"
		while [ $i -lt 3 ]
		do
			echo "$i"
			echo "salloc -p pops -N $core srun -n 1 time mpirun -host pop1,pop2,pop3,pop4,pop5,pop6,pop7,pop8 -c $proc ./$name 2>&1"
			times=$(salloc -p pops -N $core srun -n 1 time mpirun -host pop1,pop2,pop3,pop4,pop5,pop6,pop7,pop8 -c $proc ./$name 2>&1)
			result=$(echo "$times" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
			echo "result is $result"
			echo "$times"
			echo "$result" >> results.txt	
			i=$[$i+1]
		done
		contproc=$[$contproc+1]
	done
done


