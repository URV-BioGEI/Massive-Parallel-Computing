CC = gcc
CFLAGS = -O3 -Wall -fopenmp

QS_Secuencial : QS_Secuencial.c
	$(CC) $(CFLAGS) -o QS_Secuencial QS_Secuencial.c

run : QS_Secuencial
	./QS_Secuencial 300000000 2

.PHONY : clean
clean :
	-rm QS_Secuencial

