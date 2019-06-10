#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>
#include <string.h>

#define CERT 1
#define FALS 0
#define POSICIONS 8

// Ha de ser inicialment correcta !! Número de solucions esperades: 13889280
// Execució a POP en secuencial 93,5 segons
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
  MPI_Comm_size(MPI_COMM_WORLD, &total_processos);

  long long int nsol = 0, total = 0;
  int state[POSICIONS];

  for (i = 0; i < POSICIONS; i++)
    state[i] = 1;

  int flag = CERT, current_position = 4, num_solucions_explorades = 1;
  for (i = 0; i < POSICIONS; i++)
    num_solucions_explorades *= 9;

  for (i = 0; i < num_solucions_explorades; i++)  // iterem sobre espai de possibles solucions VR(9, 5) = 9^⁵
  {
    if (i % total_processos == id)
    {
      flag = CERT;
      for (j = 0; j < POSICIONS && flag; j++)  // provem solució parant quan toca
      { // HC for first empty row and column with zeros but parametrized with number of explored positions
        flag &= puc_posar(3 + (j + 4) / 9, (4 + j) % 9, state[j]); 
        taula[3 + (j + 4) / 9][(4 + j) % 9] = state[j];  // Apliquem solució encara que no poguem posar, pero sortim immediatament del bucle 
      }
      if (flag)  // si flag vol dir que Podem posar tots els valors de state a cada posicio
      {
          //printf("\n Proces %i porta %i solucions trobades i calculara solucio %i %i %i %i %i %i %i", id, num_solucio_actual, state[0], state[1], state[2], state[3], state[4], state[5], state[6]);
          nsol += recorrer(3 + (j + 4) / 9, (4 + j) % 9);  // fem calculs
      }
      while (j >= 0)  // ressetegem la taula
      {
        taula[3 + (j + 4) / 9][(4 + j) % 9] = 0;
        j--;
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
      current_position = POSICIONS - 1;  
    }
    else  // hem de sumar normal
    {
      state[current_position]++;
    }
  }
  MPI_Reduce(&nsol, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  if (id == 0)

    printf("\nnumero solucions : %lld\n", total);
  return 0;
}
