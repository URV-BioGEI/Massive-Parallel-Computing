#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>
#include <string.h>

#define CERT 1
#define FALS 0

// Ha de ser inicialment correcta !!
int taula[9][9] = \
        {1,2,3, 4,5,6,  7,8,9,  \
         9,8,7, 3,2,1,  6,5,4,  \
         6,5,4, 7,8,9,  1,2,3,  \
\
         7,9,8, 1,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
\
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0};




int puc_posar(int x, int y, int z)
{
int i,j,pi,pj;

 for (i=0;i<9;i++) if (taula[x][i] == z) return(FALS); // Files
 for (i=0;i<9;i++) if (taula[i][y] == z) return(FALS); // Columnes
 // Quadrat
 pi = x-x%3; //truncament
 pj = y-y%3; //truncament
 for (i=0;i<3;i++) 
  for (j=0;j<3;j++) 
  if (taula[pi+i][pj+j] == z) return(FALS);

 return(CERT);
}

////////////////////////////////////////////////////////////////////


int recorrer(int i, int j)
{
int k;
long int s=0;

 if (taula[i][j]) //Valor fixe no s'ha d'iterar
  {
     if (j<8) return(recorrer(i,j+1));
     else if (i<8) return(recorrer(i+1,0));
     else return(1); // Final de la taula
  }
 else // hi ha un 0 hem de provar
  {
   for (k=1;k<10;k++)
     if (puc_posar(i,j,k)) 
      {
       taula[i][j]= k; 
   if (j<8) s += recorrer(i,j+1);
   else if (i<8) s += recorrer(i+1,0);
   else s++;
       taula[i][j]= 0;
      }
  }
return(s);
}

/////////////////////////////////////////////////////////////////
int main(int nargs, char* args[])
{
  int i, j, total_processos, id;

  MPI_Init(&nargs, &args);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD,&total_processos);

  long long int nsol = 0, total = 0;
  int state[5];
  for (i = 0; i < 5; i++)
    state[i] = 1;
  int proces_objectiu = 1;

  if (id == 0)  // master
  {
    int flag = CERT, current_position = 4;
    for (i = 0; i < 9*9*9*9*9; i++)  // iterem sobre espai de solucions
    {
      flag = CERT;
      for (j = 0; j < 5 && flag; j++)  // provem solució parant quan toca
      {
        //printf("\nIntento posar a la fila %i columna %i el num %i on hi ha un %i", 3, 4 + j, state[j], taula[3][4+j]);
        flag &= puc_posar(3, 4 + j, state[j]);
      }
      if (flag)  // enviem dades als esclaus
      {
        MPI_Send(state, 5, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD);
        proces_objectiu++;
        if (proces_objectiu == total_processos)
        {
          proces_objectiu = 1;
        }
      }

      // Actualitzem estat
      if (state[current_position] == 9)  // hem de sumar portant
      {
        while (state[current_position] == 9)  // reiniciem xifres afectades
        {
          state[current_position] = 1;
          current_position--;
        }
        state[current_position]++; // incrementem el nombre al que li arriba la xifra portant
        current_position = 4;  
      }
      else  // hem de sumar normal
      {
        state[current_position]++;
      }
    }

    // Hem acabat d'enviar feina, enviem senyal de fi
    for (i = 1; i < total_processos; i++)
    {
      MPI_Send(state, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    }
    MPI_Reduce(&nsol, &total,1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    printf("\nnumero solucions : %lld\n", total);
    return 0;
  }
  else  // slave
  { 
    MPI_Status estat;
    while (CERT)
    {
      /*if (id == 1)
      {
        printf("\n\n");
        for (i = 0; i < 9; i++)
        {
          printf("\n");
          for (j = 0; j < 9; j++)
          {
            printf("%i ", taula[i][j]);
          }
        }
      }*/
      MPI_Recv(&taula[3][4], 5, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estat); 
      /*if (id == 1)
      {
        printf("\n\n");
        for (i = 0; i < 9; i++)
        {
          printf("\n");
          for (j = 0; j < 9; j++)
          {
            printf("%i ", taula[i][j]);
          }
        }
      }*/
      if (estat.MPI_TAG)  // enviem dades
      {
        MPI_Reduce(&nsol, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Finalize();
        exit(0);
      }
      else
      {
        nsol += recorrer(3, 4);
      }
    }
  }
}