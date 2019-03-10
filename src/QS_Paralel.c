#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <omp.h>

#define NN 384000000  // 384000000 int * (4 B/ 1 int) * (1 GB / 2^30 B) = 1,43 GB de dades (com a màxim) carregades a memoria
//#define MAX_INT ((int) ((unsigned int) (-1) >> 1) )  // Definim el valor màxim d'un enter segons la màquina

//(validació 3221142676932982)

int valors[NN + 1];  
int valors2[NN + 1];

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
	// o bé que el vector es molt curt 
 	// Després repartim els dos subvectors en dues crides recursives, recalculant els indexos que passem a la funció
	if (f > 1) qs(val, f);  // reordena el vector esquerra
	if (i < ne - 1) qs(&val[i], ne - f - 1);  // reordena el vector dret
}

// Funció merge
// S'encarrega de mesclar dos vectors ordenats i contigus en memoria de la mateixa mida per a obtenir un vector output ordenat, 
// situat en un altre espai de memoria.

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


int main(int nargs, char* args[])
{
	int ndades, i, m, parts, porcio;
	int *vin, *vout, *vtmp;
//	unsigned int seed = 1;
	long long sum = 0;

	assert(nargs == 3);
  
	ndades = atoi(args[1]);
	assert(ndades <= NN);
  
	parts = atoi(args[2]);
	omp_set_num_threads(parts);

	if (parts < 2) 
		assert("Han d'haver dues parts com a minim" == 0);

	if (ndades % parts) 
		assert("N ha de ser divisible per parts" == 0);

	porcio = ndades / parts;

	for (i = 0; i < ndades; i++) 
		valors[i] = rand();
		//valors[i] = rand_r(&seed);

	//printf("Ya termine con los randoms misenyor");
	// Quicksort a parts
	#pragma omp parallel for default(none) firstprivate(porcio, parts) shared(valors)
	for (i = 0; i < parts; i++)
		qs(&valors[i * porcio], porcio);

	// Merge en arbre
	vin = valors;
	vout = valors2;
	//#pragma omp parallel for ordered 
	for (m = 2 * porcio; m <= ndades; m *= 2)
	{
		#pragma omp parallel for default(none) firstprivate(m, ndades) shared(vin, vout) schedule(static)
		for (i = 0; i < ndades; i += m)
			merge2(&vin[i], m, &vout[i]);
		vtmp = vin;
		vin = vout;
    	vout = vtmp;
  	}

	// Validacio
	bool correct = true;  // Transform validation into acummulation	

	#pragma omp parallel for default(none) firstprivate(ndades, vin) schedule(static) reduction(&&:correct)
	for (i = 1; i < ndades; i++) 
		correct &= vin[i - 1] <= vin[i];

	assert(correct);

	#pragma omp parallel for default(none) firstprivate(ndades, vin) reduction(+:sum) schedule(static)
	for (i = 0; i < ndades; i += 100)
		sum += vin[i];

	printf("validacio %lld \n", sum);
	exit(0);
}

