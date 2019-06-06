CC = mpicc
CFLAGS = -O3 -Wall

all : sudoku_secuencial P2.2_CPM_MarineRuiz P2.2_CPM_Marine P2.2_CPM_Marine_Master-Slave

P2.2_CPM_MarineRuiz : P2.2_CPM_MarineRuiz.c
	$(CC) $(CFLAGS) -o P2.2_CPM_MarineRuiz P2.2_CPM_MarineRuiz.c

P2.2_CPM_Marine : P2.2_CPM_Marine.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine P2.2_CPM_Marine.c

sudoku_secuencial : sudoku_secuencial.c
	$(CC) $(CFLAGS) -o sudoku_secuencial sudoku_secuencial.c

P2.2_CPM_Marine_Master-Slave : P2.2_CPM_Marine_Master-Slave.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Master-Slave.c

run : P2.2_CPM_Marine sudoku_secuencial P2.2_CPM_Marine_Master-Slave
	bash corretodos.sh

runone : P2.2_CPM_Marine
	mpirun -np 4 P2.2_CPM_Marine

.PHONY : clean
clean :
	rm sudoku_secuencial P2.2_CPM_MarineRuiz P2.2_CPM_Marine P2.2_CPM_Marine_Master-Slave >/dev/null 2>&1
