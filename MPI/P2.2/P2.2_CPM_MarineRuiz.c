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
int i, total_processos, id;

MPI_Init(&nargs, &args);
MPI_Comm_rank(MPI_COMM_WORLD, &id);
MPI_Comm_size(MPI_COMM_WORLD,&total_processos);

long long int nsol = 0;

int pos_probades[4][2]; // array of modified positions

int num_pos_probades = 0 ;


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
i =1;
int min=0;
int max = 0;
int trobat=0;
while((i<10) & (trobat==0)){
    if(puc_posar(pos_probades[0][0], pos_probades[0][1],i,tabla)){
	min=i;
        trobat=1;
    }
    i++;
}
i=9;
while(i>0 & trobat==1){
	if(puc_posar(pos_probades[0][0], pos_probades[0][1],i,tabla)){
		max=i;
        	trobat=2;
   	 }
	i--;
}
min = (min-1)*729;
max = ((max)*729);
int datos = max-min;
int porcio = datos/total_processos;
if (porcio%total_processos != 0){
	porcio++;
}
//for (i = id+min; i < max; i=i+total_processos)
for(i=(id*porcio)+min; i<((id+1)*porcio)+min; i++)
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
MPI_Barrier(MPI_COMM_WORLD);
long long int total=0;
MPI_Reduce(&nsol, &total,1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

if(id==0) {
	printf("\nnumero solucions : %ld\n",total);
	MPI_Finalize();
	return 0;
}
else {
	MPI_Finalize();
	exit(0);
}
}
