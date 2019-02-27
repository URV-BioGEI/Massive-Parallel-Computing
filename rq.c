#include <stdio.h>
#include <stdlib.h> 

#define D 128

int r[D+2];

long long sol;

main(int argc, char* argv[])
{
int rr,i,a,b;
long long c;

 sol=0;
 c = 0;
 rr=atoi(argv[1]);
 printf("Per %d reines hi ha ",rr);
 r[1]=1;
 /*printf ("Reina 1, pos 1 \n"); */
 a=2; r[2]=1;
 /*printf ("Reina 2, pos 1 \n"); */
 while (a > 0)
  {
   c ++;
   if (r[a] > rr) /* s'han explorat totes les alternatives d'una reina */
    {
     a--;
     r[a]++;
    }
   else 
    {
     b=1;
     for (i=1;i<a;i++)
        {
	b = b && ((r[a] != r[i]) && (r[a] != r[i]+(a-i)) && (r[a] != r[i]+(i-a)));
        /*printf("\t r[%d]:%d cmp r[%d]:%d =+-%d bool:%d\n",a,r[a],i,r[i],(a-i),b);*/
        }
     if (b)   
    	 {
          if (a == rr) /* s'han explorat les totes les reines sense conflicte */
	   {
            sol++; /*psol(r,rr); */
            r[a]++;
	   }
	  else  /* de moment OK, pasem a la seguent reina*/
           {
	    a++;
	    r[a]=1;
    	   }
         }
   	else /* alternativa incorrecta, mirem la seguent */
    	 {
	  r[a]++;
          /*printf("Reina %d, pos %d \n",a,r[a]); */
	 }
    } 
  } 
 printf("%lld solucions\n",sol);
 printf("%lld voltes\n",c);
}

psol(int* r,int nr)
{
int i;

 printf("Sol: ");
 for(i=1;i<=nr;i++) printf("%d ",r[i]);
 printf("\n");
 exit(0);
}
