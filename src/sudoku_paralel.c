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


int puc_posar(int x, int y, int z)
{
	static int i,j,pi,pj;
	int boolean=CERT;
	#pragma omp threadprivate(i,j,pi,pj)
	#pragma omp parallel default(none) copyin(i,j,pi,pj) private(taula) shared(stdout, x, y, z,boolean)
	{
	#pragma omp sections
	{
	#pragma omp section
	{
	for (i=0;i<9;i++) if (taula[x][i] == z) boolean=FALS; // Files	
	}
	#pragma omp section
	{
	for (i=0;i<9;i++) if (taula[i][y] == z) boolean=FALS; // Columnes
 	}
	// Quadrat
	#pragma omp section
	{
	pi = (x/3)*3; //truncament
	pj = y-y%3; //truncament
 	for (i=0;i<3;i++) 
  		for (j=0;j<3;j++) 
			if (taula[pi+i][pj+j] == z) boolean=FALS;
	}
	}
	}	
	if(boolean==FALS)
	{
 		printf("%d, %d, %d\n", x,y,z);
		fflush(stdout);

		return(FALS);
	}
	return(CERT);
	
}

////////////////////////////////////////////////////////////////////
int recorrer( int i, int j)
{
	int k;
	long int s=0;
	//static int m,n;
	
	//#pragma omp threadprivate(m,n)	
 	if (taula[i][j]) //Valor fixe no s'ha d'iterar
  	{
     		if (j<8) return(recorrer(i,j+1));
     		else if (i<8) return(recorrer(i+1,0));
     		else return(1); // Final de la taula
  	}
 	else // hi ha un 0 hem de provar
  	{
   		//m=i;
		//n=j;
		//#pragma omp parallel for schedule(dynamic) default(none) copyin(m,n) shared(stdout,s) firstprivate(taula) 
	   	for (k=1;k<10;k++)
   		{
    			if (puc_posar(i,j,k))
     			{
       				//printf("%d %d %d\n",i,j,k);
				//fflush(stdout);
				taula[i][j]= k; 
	 			if (j<8) s += recorrer(i,j+1);
	 			else if (i<8) s += recorrer(i+1,0);
				else s++;
      				taula[i][j]= 0;
     			}
			//printf("%ld\n", s);
			//fflush(stdout);
			
   		}
  	}
	return(s);
}

////////////////////////////////////////////////////////////////////
int main()
{	
	//#pragma omp threadprivate(taula)
	int i, j;	
	long int nsol;
	omp_set_num_threads(1);
	i=0;
	j=0; 
	nsol = recorrer(i,j);
	printf("numero solucions : %ld\n",nsol);
	exit(0);
}
