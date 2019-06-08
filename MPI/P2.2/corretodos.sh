
#!/bin/bash
rm results.txt
nodes=('2' '2' '4' '4' '8' '8' '8' )
procesos=('2' '4' '4' '8' '8' '16' '32')
npernodes=('2' '4')
binari=('P2.2_CPM_Marine P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Depth7 P2.2_CPM_Marine_Non-Balanced P2.2_CPM_Marine_Non-Balanced_Depth7');
for name in ${binari[@]}
do
	echo "$name" >> results.txt
	echo "$name"
	contproc=0
	for core in ${nodes[@]}
	do
		proc=${procesos[$contproc]}
		echo "$core $proc"
		echo "$core $proc" >> results.txt
		i="0"
		while [ $i -lt 3 ]
		do
			times=$(salloc -p pops -N $core srun -n 1 time mpirun -host pop1,pop2,pop3,pop4,pop5,pop6,pop7,pop8 -c $proc ./$name 2>&1)
			result=$(echo "$times" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
			python -c "print \"$result \"+ str(93.5 / $result)" | tee -a results.txt	
			i=$[$i+1]
		done
		contproc=$[$contproc+1]
	done
	echo "8 x 2 64"
	echo "8 x 2 64" >> results.txt
	i="0"
	while [ $i -lt 3 ]
	do
		times=$(salloc -p pops -N 8 srun -n 1 time mpirun -host pop1,pop2,pop3,pop4,pop5,pop6,pop7,pop8 -c 64 -npernode 8 ./$name 2>&1)
		result=$(echo "$times" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
		python -c "print \"$result \"+ str(93.5 / $result)" | tee -a results.txt
		i=$[$i+1]
	done
	echo "8 x 4 128"
	echo "8 x 4 128" >> results.txt
	i="0"
	while [ $i -lt 3 ]
	do
		times=$(salloc -p pops -N 8 srun -n 1 time mpirun -host pop1,pop2,pop3,pop4,pop5,pop6,pop7,pop8 -c 128 -npernode 16 ./$name 2>&1)
		result=$(echo "$times" | grep "elapsed" | cut -d ' ' -f3 | cut -c 3-7)
		python -c "print \"$result \"+ str(93.5 / $result)" | tee -a results.txt		
		i=$[$i+1]
	done
done


