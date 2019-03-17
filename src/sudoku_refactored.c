#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <omp.h>


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

#pragma omp threadprivate(taula)

int puc_posar(int x, int y, int z)
{
int i,j,pi,pj;

 for (i=0;i<9;i++) if (taula[x][i] == z) return(FALS); // Files
 for (i=0;i<9;i++) if (taula[i][y] == z) return(FALS); // Columnes
 // Quadrat
 pi = (x/3)*3; //truncament
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

////////////////////////////////////////////////////////////////////
int main(int nargs, char* args[])
{
int i,j,v, d = 0, depth = 1;
long int nsol = 0;

int parts_minimes = atoi(args[1]);
omp_set_num_threads(parts_minimes);

for (i = 0; i < 9; i++)
{
  for (j = 0; j < 9 && d < depth; j++)
  {
    if (!taula[i][j])
    {
      //push
      d++;
      #pragma omp parallel for reduction(+:nsol) schedule(dynamic) copyin(taula)
      for (v = 0; v < 9; v++)
      {
        if (puc_posar(i, j, v))
        {
          taula[i][j] = v;
          nsol += recorrer(i, j);
        }
        //taula[i][j] = v
      }
      //pop
    }
  }
}


/*
taula[3][4] = 3;
nsol = recorrer(0, 0);
taula[3][4] = 4;
nsol += recorrer(0, 0);
taula[3][4] = 6;
nsol += recorrer(0, 0);

printf("numero solucions idea: %ld\n",nsol);
taula[3][4] = 0;
nsol = recorrer(0,0); */
printf("numero solucions : %ld\n",nsol);
exit(0);
}
