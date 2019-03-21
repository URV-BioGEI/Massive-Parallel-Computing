CC = gcc
CFLAGS = -O3 -Wall -fopenmp

all : sudoku_refactored sudoku_secuencial sudoku_paralel sudoku_refactored3

sudoku_refactored : sudoku_refactored.c
	$(CC) $(CFLAGS) -o sudoku_refactored sudoku_refactored.c

sudoku_refactored3 : sudoku_refactored3.c
	$(CC) $(CFLAGS) -o sudoku_refactored3 sudoku_refactored3.c

sudoku_secuencial : sudoku_secuencial.c
	$(CC) $(CFLAGS) -o sudoku_secuencial sudoku_secuencial.c

sudoku_paralel : sudoku_paralel.c
	$(CC) $(CFLAGS) -o sudoku_paralel sudoku_paralel.c

run : sudoku_refactored
	time ./sudoku_refactored

runSecuencial : sudoku_secuencial
	time ./sudoku_secuencial

.PHONY : clean
clean :
	-rm sudoku_secuencial sudoku_refactored sudoku_paralel
