CC = gcc
CFLAGS = -O3 -Wall -fopenmp

all : sudoku_secuencial P1.2_CPM_MarineRuiz

P1.2_CPM_MarineRuiz : P1.2_CPM_MarineRuiz.c
	$(CC) $(CFLAGS) -o P1.2_CPM_MarineRuiz P1.2_CPM_MarineRuiz.c

sudoku_secuencial : sudoku_secuencial.c
	$(CC) $(CFLAGS) -o sudoku_secuencial sudoku_secuencial.c

run : P2.1_CPM_MarineRuiz
	time ./P1.2_CPM_MarineRuiz

runSecuencial : sudoku_secuencial
	time ./sudoku_secuencial

.PHONY : clean
clean :
	rm sudoku_secuencial P2.1_CPM_MarineRuiz
