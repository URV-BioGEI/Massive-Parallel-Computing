CC = gcc
CFLAGS = -O3 -Wall -fopenmp

all : QS_Secuencial QS_Secuencial_Refactored QS_Paralel

QS_Secuencial : QS_Secuencial.c
	$(CC) $(CFLAGS) -o QS_Secuencial QS_Secuencial.c

QS_Secuencial_Refactored : QS_Secuencial_Refactored.c
	$(CC) $(CFLAGS) -o QS_Secuencial_Refactored QS_Secuencial_Refactored.c

QS_Paralel : QS_Paralel.c
	$(CC) $(CFLAGS) -o QS_Paralel QS_Paralel.c

run_S : QS_Secuencial
	./QS_Secuencial 300000000 2

run_P : run

run : QS_Paralel
	./QS_Paralel 300000000 2

time : QS_Paralel
	time ./QS_Paralel 300000000 2

time_S : QS_Secuencial
	time ./QS_Secuencial 300000000 2

time_P : time
	

.PHONY : clean
clean :
	-rm QS_Secuencial QS_Secuencial_Refactored QS_Paralel

