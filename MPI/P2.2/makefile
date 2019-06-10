CC = mpicc
CFLAGS = -O3 -Wall

all : sudoku_secuencial P2.2_CPM_MarineRuiz P2.2_CPM_Marine_Depth6 P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Depth7 P2.2_CPM_Marine_Depth8 P2.2_CPM_Marine_Non-Balanced_Depth6 P2.2_CPM_Marine_Non-Balanced_Depth7 P2.2_CPM_Marine_Non-Balanced_Depth8

P2.2_CPM_MarineRuiz : P2.2_CPM_MarineRuiz.c
	$(CC) $(CFLAGS) -o P2.2_CPM_MarineRuiz P2.2_CPM_MarineRuiz.c

P2.2_CPM_Marine_Depth6 : P2.2_CPM_Marine_Depth6.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Depth6 P2.2_CPM_Marine_Depth6.c

sudoku_secuencial : sudoku_secuencial.c
	$(CC) $(CFLAGS) -o sudoku_secuencial sudoku_secuencial.c

P2.2_CPM_Marine_Master-Slave : P2.2_CPM_Marine_Master-Slave.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Master-Slave.c

P2.2_CPM_Marine_Depth7 : P2.2_CPM_Marine_Depth7.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Depth7 P2.2_CPM_Marine_Depth7.c

P2.2_CPM_Marine_Depth8 : P2.2_CPM_Marine_Depth8.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Depth8 P2.2_CPM_Marine_Depth8.c

P2.2_CPM_Marine_Non-Balanced_Depth6 : P2.2_CPM_Marine_Non-Balanced_Depth6.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Non-Balanced_Depth6 P2.2_CPM_Marine_Non-Balanced_Depth6.c

P2.2_CPM_Marine_Non-Balanced_Depth7 : P2.2_CPM_Marine_Non-Balanced_Depth7.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Non-Balanced_Depth7 P2.2_CPM_Marine_Non-Balanced_Depth7.c

P2.2_CPM_Marine_Non-Balanced_Depth8 : P2.2_CPM_Marine_Non-Balanced_Depth8.c
	$(CC) $(CFLAGS) -o P2.2_CPM_Marine_Non-Balanced_Depth8 P2.2_CPM_Marine_Non-Balanced_Depth8.c

run : P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Depth6 P2.2_CPM_Marine_Depth8 P2.2_CPM_Marine_Depth7 P2.2_CPM_Marine_Non-Balanced_Depth6 P2.2_CPM_Marine_Non-Balanced_Depth7 P2.2_CPM_Marine_Non-Balanced_Depth8 sudoku-secuencial
	bash corretodos.sh

runone : P2.2_CPM_Marine_Depth6
	mpirun -np 4 P2.2_CPM_Marine_Depth6

.PHONY : clean
clean :
	rm sudoku_secuencial P2.2_CPM_MarineRuiz P2.2_CPM_Marine_Depth6 P2.2_CPM_Marine_Master-Slave P2.2_CPM_Marine_Depth7 P2.2_CPM_Marine_Depth8 P2.2_CPM_Marine_Non-Balanced_Depth6 P2.2_CPM_Marine_Non-Balanced_Depth7 P2.2_CPM_Marine_Non-Balanced_Depth8
