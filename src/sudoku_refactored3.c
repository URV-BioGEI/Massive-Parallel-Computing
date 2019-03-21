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

pi = (x/3)*3; //truncament
pj = y-y%3; //truncament
 // Quadrat
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
int i;
long int nsol = 0;

omp_set_num_threads(atoi(args[1]));
int depth = atoi(args[2]);


int pos_probades[depth][2]; // array of modified positions

int num_pos_probades = 0;


for (i = 0; i < 81 && num_pos_probades<depth; i++)
{
    if (!taula[i/9][i%9])
    {
      pos_probades[num_pos_probades][0] = i/9;
      pos_probades[num_pos_probades][1] = i%9;
      num_pos_probades++;
   }
}
#pragma omp parallel for default(none) schedule(dynamic) shared(pos_probades) copyin(taula) reduction(+:nsol)
for (i = 0; i < 729; i++)
{
    if (puc_posar(pos_probades[0][0], pos_probades[0][1], ((i/81)%9)+1))
    {
      taula[pos_probades[0][0]][pos_probades[0][1]] = ((i/81)%9)+1;
      if (puc_posar(pos_probades[1][0], pos_probades[1][1], ((i/9)%9)+1))
      {
        taula[pos_probades[1][0]][pos_probades[1][1]] = ((i/9)%9)+1;
        
          if (puc_posar(pos_probades[2][0], pos_probades[2][1], (i%9)+1))
          {
           
            taula[pos_probades[2][0]][pos_probades[2][1]] = (i%9)+1;
            nsol+=recorrer(0,0);
            taula[pos_probades[2][0]][pos_probades[2][1]] = 0; 
          }
        taula[pos_probades[1][0]][pos_probades[1][1]] = 0;

      }
    taula[pos_probades[0][0]][pos_probades[0][1]] = 0;
   }
  
}
printf("\nnumero solucions : %ld\n",nsol);
exit(0);
}
