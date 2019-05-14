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

// Funció merge reescrita

void merge2(int* val, int n,int *vo)
 {
  int i,posi,posj;

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
	//printf("rili");
	int id, ndades = atoi(args[1]), total_processos;

	MPI_Init(&nargs, &args);	// Inicialitzem entorn paralel
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 	// Obtenim nombre de processos
	MPI_Comm_size(MPI_COMM_WORLD, &total_processos); 	// Obtenim el numero total de processos

	int i, proces_objectiu, porcio = ndades / total_processos;
	int *vin, *vtmp, *vin2, *valors, *valors2;
	long long sum = 0;
	MPI_Status estat;
	//MPI_Request request;

	// Compute sizes of vectors
	if (id != 0)  // process 0 is the only one that does not adapt to the mathematical expression to compute sizes
	{
		for (i = 2; i <= total_processos; i *= 2)
		{
			if (id % i > 0)  // Si no és múltiple de l'actual potencia de dos
			{
				valors = malloc(porcio * (i / 2) * sizeof(int));  // reservem una mida de porcio * i/2. la mida
				valors2 = malloc(porcio * (i / 2) * sizeof(int));
				break;
			}
		}
	}
	else  // reserva mida total pel proces 0
	{
		valors = malloc(ndades * sizeof(int));
		valors2 = malloc(ndades * sizeof(int));
	}

	// Vector initialization
	for (i = 0; i < porcio * id; i++)  // llamadas a rand para actualizar el estado
			rand();
	for (i = 0; i < porcio; i++)  // llamadas productivas, donde almacenamos
		valors[i] = rand();

	qs(&valors[0], porcio);

	vin = valors;
	vin2 = valors2;

    for (i = 2; i < total_processos * 2; i *= 2)
	{
		if (id % i > 0)
		{
			proces_objectiu = id - id % i;
			MPI_Send(vin, i / 2 * porcio, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD);
			MPI_Finalize();
			exit(0);
		}
		else
		{
			proces_objectiu = id + i / 2;
			MPI_Recv(&vin[i / 2 * porcio], i / 2 * porcio, MPI_INT, proces_objectiu, 0, MPI_COMM_WORLD, &estat);  // rebo porcio * i / 2 dades, que no consulto perque ja ho se
			merge2(vin, porcio * i, vin2);

			vtmp = vin;  // intercambiem punters
			vin = vin2;
			vin2 = vtmp;

		}
	}
		bool correcte = true;
		for (i = 1; i < ndades; i++)
			correcte &= vin[i - 1] <= vin[i];
		assert(correcte);
		for (i = 0; i < ndades; i += 100)
		{
			sum +=vin[i];
		}
		printf("validacio %lld \n", sum);
		MPI_Finalize();
		return 0;
		
}
