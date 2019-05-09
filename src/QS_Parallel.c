#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  
#include <mpi.h>
#include <stdbool.h>
#include <string.h>

// Funció quicksort
// int pointer *val: Punter que apunta a un vector de dades compartit en memoria. 
// int ne: Indica la quantitat de dades que podem modificar del nostre vector apuntat per *val.
// per tant l'índex màxim per a accedir al vector *val és ne - 1 
void qs(int *val, int ne)
{
	int i, f;  // i es l'index per l'esquerra, f index per la dreta 
	int pivot, vtmp, vfi;  // valor pivot contra el que ordenarem, vtmp valor que estem ordenant, vfi variable auxiliar que conté el valor a ordenar
	//quan acabem de colocar un valor DESPRÉS del pivot, ja que no tenim espais buits i la necessitem.

	pivot = val[0];  // Inicialment escollim arbitrariament el valor de la primera posicio com a pivot
	i = 1;  // índex mínim per a accedir al vector (comencem a i = 1 perquè a i = 0 ja hi tenim al pivot)
	f = ne - 1;  // índex màxim per a accedir al nostre vector global
	vtmp = val[i];  // emmagatzemmem de manera temporal el valor a la posició val[1] per a tenir el primer valor a col·locar

	while (i <= f)  // mentre el índex inicial no superi a l'índex final
	{
		if (vtmp < pivot)  // Si el valor temporal (el que estem ordenant) és més petit que el pivot...
		{
			val[i - 1] = vtmp;  // Movem el valor temporal a la posició que acabem de deixar lliure (a la primera iteració al lloc on estava el pivot)
			i++;  // passem al següent element 
			vtmp = val[i];  // Ara que hem colocat el vtmp carreguem el següent valor a ordenar
		}
		else  // si es mes gran
		{
			vfi = val[f];  // guardem un valor a la dreta del pivot per a fer lloc per al nou valor
			val[f] = vtmp;  // guardem el valor que estavem ordenant
			f--;  // passem al següent element
			vtmp = vfi;  // emmagatzemmem el valor temporal com a nou valor a ordenar
		}
	}
	val[i - 1] = pivot;  // Un cop els dos índexos es creuen (i > f, i - 1 = f) coloquem el pivot a la posició i - 1 per a que els dos 
	// sub vectors es trobin ordenats
 
 	// En principi aquests dos ifs s'activen i porten dues trucades recursives que reordenen els dos subvectors
 	// Si no es aixi, vol dir que el pivot escollit era major o menor que la resta d'elements en l'array (es el pitjor cas al escollir un pivot) 
 	// Després repartim els dos subvectors en dues crides recursives, recalculant els indexos que passem a la funció
	if (f > 1) qs(val, f);  
	if (i < ne - 1) qs(&val[i], ne - f - 1);
}
void merge2(int* val, int n, int *vo)
{
	int i, posi, posj; 

	posi = 0;  // posicio inicial en el vector esquerra
	posj = (n / 2);  // posicio inicial en el vector dret //? redundant parenthesis
 
	for (i = 0; i < n; i++)  // recorrem el conjunt dels dos vectors
		// si hem acabat amb el vector j o no hem acabat el vector i & el valor al vector i es menor o igual que el valor al vector j 
		if (((posi < n/2) && (val[posi] <= val[posj])) || (posj >= n)) 
			vo[i] = val[posi++];
		else 
			vo[i] = val[posj++];
}


int main(int nargs,char* args[])
{
	//printf("rili");
	int closest_powerof2 = 0, ideal_num_processos, total_processos, id, ndades = atoi(args[1]), parts = atoi(args[2]);
	for (ideal_num_processos = 1; ideal_num_processos <= parts; ideal_num_processos *= 2) closest_powerof2++; 
	
	MPI_Init(&nargs, &args);	// Inicialitzem entorn paralel
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 	// Obtenim nombre de processos 
	MPI_Comm_size(MPI_COMM_WORLD, &total_processos); 	// Obtenim el numero total de processos 
	int x=2;
	while(id%x==0 && x<parts)
	{
		x=x*2;
	}
	int num_received_values, i, counter = 0, proces_objectiu, porcio = ndades / parts, residu = ndades % parts, acumulador = 0;
	int *vin, *vout, *vtmp, *vin2;
	int * valors=malloc(((x*porcio)+1)*sizeof(int));  
	int * valors2=malloc(((x*porcio)+1)*sizeof(int));  
	//int * salida = malloc(((porcio*2)+1)*sizeof(int));
	long long sum = 0;
	MPI_Status estat;
	MPI_Request request;
	int *num_elements = malloc(sizeof(int) * parts);
	int *offsets = malloc(sizeof(int) * parts);
	int a=0;
	// Vector initialization
        for (i = 0; i < ndades; i++){
		if((i/porcio)==id){
        		valors[i%porcio] = rand();
		}
		else{
			a = rand();
		}
	}
	qs(&valors[0], porcio);
	vin = valors;
	vout= valors2;
	//vout= salida;
        for (i=2; i<total_processos*2; i*=2)
	{
		if(id%i > 0)
		{
			proces_objectiu=id-id%i;
			//printf("Soy %d, quiero enviar %d a %d, MUERO \n", id, i/2*porcio, proces_objectiu); 
			//fflush(stdout);
			//if (i==4){
				
			//	MPI_Send(vin, i*porcio, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD);
			//}
		        //else{
			MPI_Send(vin, i/2*porcio, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD);
			//}
			//printf("Soy %d, he enviado %d a %d, MUERO \n", id, i/2*porcio, proces_objectiu); 
			//fflush(stdout);
			MPI_Finalize();
			exit(0);
		}
		else
		{
			proces_objectiu=id+i/2;
			if(proces_objectiu != id && proces_objectiu < parts)
			{	
				//printf("Soy %d, quiero recibir %d de %d, espero \n", id, i/2*porcio, proces_objectiu); 
			        //fflush(stdout);
				MPI_Recv(&vin[i/2*porcio], ndades/2, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD, &estat);
				//printf("Soy %d, he recibido %d de %d, espero \n", id, i/2*porcio, proces_objectiu); 
			        //fflush(stdout);
				merge2(vin, i*porcio, vout);
				//printf("Soy %d, he mergeado %d con %d \n", id, i*porcio, proces_objectiu); 
			        //fflush(stdout);
				vtmp=vin;
				vin=vout;
				vout=vtmp;
			}
		}
	}
	for(i=1;i<ndades;i++) assert( vin[i-1]<=vin[i]);
	for(i=0;i<ndades; i+=100) sum+=vin[i];
	printf("validacio %lld \n", sum);
	
	MPI_Finalize();
	
	return 0;
}
