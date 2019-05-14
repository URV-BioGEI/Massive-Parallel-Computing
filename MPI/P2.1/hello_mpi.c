#include <stdio.h>
#include <string.h>
#include <mpi.h>
int main(int num_args, char* args[ ]) {
     /* rank del proces    */
     int    el_meu_rank;
     /* numero de processos        */
     int    p;
     /* rank de l'emissor */
     int    font;
     /* rank del receptor */
     int    desti;
     /* etiqueta dels missatges */
     int    etiq = 0;
     /* espai per al missatge      */
     char missatge[100];    
     /* estat de la recepcio       */
     MPI_Status  estat;

/* Inicialitzar MPI */
   MPI_Init(&num_args, &args);
    /* Obtenir el rank del proces  */
   MPI_Comm_rank(MPI_COMM_WORLD, &el_meu_rank);
    /* Obtenir el numero total de processos */
   MPI_Comm_size(MPI_COMM_WORLD, &p);

                     
if (el_meu_rank != 0) {
 /* Crear el missatge */
        sprintf(missatge, "Salutacions del proces %d ",el_meu_rank);
        desti = 0;
  /* Usat strlen+1 per transmetre tambe el sentinella '\0' */
        
        sleep(3);
        MPI_Send(missatge, strlen(missatge)+1, MPI_CHAR, 
            desti, etiq, MPI_COMM_WORLD);
    }
  else { /* el_meu_rank == 0 */
        for (font = 1; font < p; font++) {
            MPI_Recv(missatge, 100, MPI_CHAR, font, etiq, 
                MPI_COMM_WORLD, &estat);
            printf("%s\n", missatge);
        }
    }
       /* Finalitzar MPI */
    MPI_Finalize();
    return (0);
} /* main */
