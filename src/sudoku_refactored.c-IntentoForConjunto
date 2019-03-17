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
//int taula[9][9] = \
        {1,2,3, 4,5,6,  7,8,9,  \
         9,8,7, 3,2,1,  6,5,4,  \
         6,5,4, 7,8,9,  1,2,3,  \
\
         7,9,8, 1,3,2,  4,6,5,  \
         2,1,5, 6,4,7,  3,9,8,  \
         0,0,0, 0,0,0,  0,0,0,  \
\
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0};



int puc_posar(int x, int y, int z)
{
int i,j;
static int pi,pj;
//#pragma omp threadprivate (pi,pj) 
// Quadrat
 pi = (x/3)*3; //truncament
 pj = y-y%3; //truncament
//#pragma omp parallel for default(none) firstprivate(x,y,z) copyin(pi,pj) shared(taula,bool)
 for (i=0;i<9 ;i++)
 {
 if (taula[x][i] == z) return(FALS); // Files
 if (taula[i][y] == z) return(FALS); // Columnes
 if (taula[pi+(i/3)][pj+(i%3)] == z) return(FALS);
 }

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
        //printf("%d, %d, %d\n", i,j,k);
        //fflush(stdout);
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
int main()
{
int i,j,k;
long int nsol;
omp_set_num_threads(4);

nsol = recorrer(0,0);
printf("numero solucions : %ld\n",nsol);
exit(0);
}
