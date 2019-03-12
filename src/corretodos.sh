#!/bin/bash

i="0"
while [ $i -lt 10 ]
do
	T1=$({ srun -p gat time ./QS_Paralel 300000000 8 >> ./LOG; } 2>&1 )
	echo "$T1"	
	i=$[$i+1]
done
i="0"
while [ $i -lt 10 ]
do
	T1=$({ srun -p gat time ./Validation_For 300000000 8 >> ./LOG; } 2>&1 )
	echo "$T1"	
	i=$[$i+1]
done
i="0"
while [ $i -lt 10 ]
do
	T1=$({ srun -p gat time ./Validation_ForNoWait 300000000 8 >> ./LOG; } 2>&1 )
	echo "$T1"	
	i=$[$i+1]
done
i="0"
while [ $i -lt 10 ]
do
	T1=$({ srun -p gat time ./Validation_Sections 300000000 8 >> ./LOG; } 2>&1 )
	echo "$T1"	
	i=$[$i+1]
done
