#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  

#define NN 384000000
#define MAX_INT ((int)((unsigned int)(-1)>>1))

int valors[NN+1];
int valors2[NN+1];

void qs(int *val, int ne)
 {
  int i,f,j;
  int pivot,vtmp,vfi;

  pivot = val[0];
  i = 1;
  f = ne-1; 
  vtmp = val[i];

  while (i <= f)
   {
    if (vtmp < pivot) {
	    val[i-1] = vtmp;
	    i ++;
	    vtmp = val[i];
	}
    else {
            vfi = val[f];
	    val[f] = vtmp;
	    f --;
	    vtmp = vfi;
	}
   }
  val[i-1] = pivot;
 
  if (f>1) qs(val,f);
  if (i < ne-1) qs(&val[i],ne-f-1);
 }

void merge2(int* val, int n,int *vo)
 {
  int vtmp;
  int i,j,posi,posj; 

   posi = 0;
   posj = (n/2);
 
   for (i=0;i<n;i++)
      if (((posi < n/2) && (val[posi] <= val[posj])) || (posj >= n))
	  vo[i] = val[posi++];
	else if (posj < n)
	  vo[i] = val[posj++];
 }

int main(int nargs,char* args[])
{
  int ndades,i,m,parts,porcio;
  int *vin,*vout,*vtmp;
  long long sum=0;

  assert(nargs == 3);
  
  ndades = atoi(args[1]);
  assert(ndades <= NN);
  
  parts = atoi(args[2]);
  if (parts < 2) assert("Han d'haver dues parts com a minim" == 0);
  if (ndades % parts) assert("N ha de ser divisible per parts" == 0);

  for(i=0;i<ndades;i++) valors[i]=rand()%MAX_INT;

porcio = ndades/parts;

// Quicksort a parts
  for (i=0;i<parts;i++)
    qs(&valors[i*porcio],porcio);

// Merge en arbre
  vin = valors;
  vout = valors2;
  for (m = 2*porcio; m <= ndades; m *= 2)
  {
    for (i = 0; i < ndades; i += m)
    	merge2(&vin[i],m,&vout[i]);
    vtmp=vin;
    vin=vout;
    vout=vtmp;
  }

// Validacio
  for (i=1;i<ndades;i++) assert(vin[i-1]<=vin[i]);
  for (i=0;i<ndades;i+=100)
    sum += vin[i];
  printf("validacio %lld \n",sum);
  exit(0);
}

