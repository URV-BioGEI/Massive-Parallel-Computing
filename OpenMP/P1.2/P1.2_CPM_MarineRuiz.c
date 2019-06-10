#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <omp.h>
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




int puc_posar(int x, int y, int z, int tabla[9][9])
{
int i,j,pi,pj;
for (i=0;i<9;i++) if (tabla[x][i] == z) return(FALS); // Files
 
for (i=0;i<9;i++) if (tabla[i][y] == z) return(FALS); // Columnes

pi = (x/3)*3; //truncament
pj = y-y%3; //truncament
 // Quadrat
  for (i=0;i<3;i++) 
   for (j=0;j<3;j++) 
	if (tabla[pi+i][pj+j] == z) return(FALS);


 return(CERT);

}

////////////////////////////////////////////////////////////////////


int recorrer(int i, int j, int tabla[9][9])
{
int k;
long int s=0;

 if (tabla[i][j]) //Valor fixe no s'ha d'iterar
  {
     if (j<8) return(recorrer(i,j+1, tabla));
     else if (i<8) return(recorrer(i+1,0, tabla));
     else return(1); // Final de la taula
  }
 else // hi ha un 0 hem de provar
  {
   for (k=1;k<10;k++)
     if (puc_posar(i,j,k, tabla)) 
      {
       tabla[i][j]= k; 
	 if (j<8) s += recorrer(i,j+1, tabla);
	 else if (i<8) s += recorrer(i+1,0,tabla);
	 else s++;
       tabla[i][j]= 0;
      }
  }
return(s);
}

/////////////////////////////////////////////////////////////////
int main(int nargs, char* args[])
{
int i;
long int nsol = 0;

omp_set_num_threads(32);

int pos_probades[4][2]; // array of modified positions

int num_pos_probades = 0;


for (i = 0; i < 81 && num_pos_probades<4; i++)
{
    if (!taula[i/9][i%9])
    {
      pos_probades[num_pos_probades][0] = i/9;
      pos_probades[num_pos_probades][1] = i%9;
      num_pos_probades++;
   }
}
int tabla[9][9];
memcpy(tabla,taula,sizeof(tabla));
#pragma omp parallel for default(none) schedule(dynamic) shared(pos_probades) firstprivate(tabla) reduction(+:nsol)
for (i = 0; i < 6561; i++)
{
 if (puc_posar(pos_probades[0][0], pos_probades[0][1], (i/729)+1,tabla))
  {
    tabla[pos_probades[0][0]][pos_probades[0][1]] = (i/729)+1;
 
    if (puc_posar(pos_probades[1][0], pos_probades[1][1], ((i/81)%9)+1,tabla))
    {
      tabla[pos_probades[1][0]][pos_probades[1][1]] = ((i/81)%9)+1;
      if (puc_posar(pos_probades[2][0], pos_probades[2][1], ((i/9)%9)+1,tabla))
      {
        tabla[pos_probades[2][0]][pos_probades[2][1]] = ((i/9)%9)+1;
          if (puc_posar(pos_probades[3][0], pos_probades[3][1], (i%9)+1,tabla))
          {
            tabla[pos_probades[3][0]][pos_probades[3][1]] = (i%9)+1;
            nsol+=recorrer(pos_probades[3][0],pos_probades[3][1],tabla);
            tabla[pos_probades[3][0]][pos_probades[3][1]] =0 ; 
          }
        tabla[pos_probades[2][0]][pos_probades[2][1]] =0 ;

      }
    tabla[pos_probades[1][0]][pos_probades[1][1]] =0 ;
   }
  tabla[pos_probades[0][0]][pos_probades[0][1]] =0 ;
 }
}

printf("\nnumero solucions : %ld\n",nsol);
exit(0);
}
