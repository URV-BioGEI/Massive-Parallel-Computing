#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  
#include <mpi.h>
#include <stdbool.h>


#define NN 250000  // 250000000 int * (4 B/ 1 int) * (1 GB / 2^30 B) = 0,93 GB de dades (com a màxim) carregades a memoria
#define MAX_INT ((int) ((unsigned int) (-1) >> 1) )  // Definim el valor màxim d'un enter segons la màquina

int valors[NN + 1];  
int valors2[NN + 1];
int valors3[NN + 1];

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

// Funció merge
// 
void merge2(int* val, int n, int *vo)
{
	int i, posi, posj; 

	posi = 0;  // posicio inicial en el vector esquerra
	posj = n / 2;  // posicio inicial en el vector dret //? redundant parenthesis
 
	for (i = 0; i < n; i++)  // recorrem el conjunt dels dos vectors
		// si hem acabat amb el vector j o no hem acabat el vector i & el valor al vector i es menor o igual que el valor al vector j 
		if (((posi < n/2) && (val[posi] <= val[posj])) || (posj >= n)) 
			vo[i] = val[posi++];
		else 
			vo[i] = val[posj++];
}

// Funció merge reescrita
// 
int merge2_different(int* in1, int n_in1, int* in2, int n_in2, int *vo)
{
	int i, posi = 0, posj = 0; 
 
	for (i = 0; i < n_in1 + n_in2; i++)  // recorrem el conjunt dels dos vectors
		// si hem acabat amb el vector j o no hem acabat el vector i & el valor al vector i es menor o igual que el valor al vector j 
		if ( ((posi < n_in1) && (in1[posi] <= in2[posj])) || (posj >= n_in2)) 
			vo[i] = in1[posi++];
		else 
			vo[i] = in2[posj++];
	return i;
}

int main(int nargs,char* args[])
{
	int ndades, i, nivell, parts = 11, porcio = NN / parts, residu = NN % parts, acumulador = 0;
	int *vin, *vout, *vtmp;
	long long sum = 0;

	int closest_powerof2 = 0;
	int ideal_num_processos = 1;
	for (i = parts; i >= 1; i /= 2 )
			closest_powerof2++;
	for (i = 0; i < closest_powerof2; i ++)
	{
		ideal_num_processos *= 2;
	}
	/* rank del proces    */
	int id;
	/* numero de processos        */
	int total_processos;
	/* rank de l'emissor */
	int proces_font;
	/* rank del receptor */
	int proces_desti;
	/* etiqueta dels missatges */
	int tag = 0;
	/* espai per al missatge      */
	char msg[100];    
	/* estat de la recepcio       */
	MPI_Status estat;
	/* estat de la request*/
	MPI_Request request;
  
  	/* Inicialitzar MPI 
	· argc Pointer to the number of arguments 
	· argv Pointer to the argument vector 
	This routine must be called by one thread only. 
	That thread is called the main thread and must be the thread that calls MPI_Finalize. */
	MPI_Init(&nargs, &args);
    /* Obtenir el rank del proces  */
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	/* Obtenir el numero total de processos */
	MPI_Comm_size(MPI_COMM_WORLD, &total_processos);
  	
	//parts = atoi(args[1]);
	int *num_elements = malloc(sizeof(int) * parts);
	int *offsets = malloc(sizeof(int) * parts);

    for (i = 0; i < parts; i++) 
    {
        num_elements[i] = porcio;
        if (residu > 0) 
        {
            num_elements[i]++;
            residu--;
        }
        offsets[i] = acumulador;
        acumulador += num_elements[i];
	}
	if (id == 0)
	{
		for (i = 0; i < NN; i++) 
			valors[i] = rand();
		printf("\n%d rand inicializtion done", id);
	}
	/* Enviar porcions
	sendbuf: address of send buffer (choice, significant only at root) 
	sendcount: integer array (of length group size) specifying the number of elements to send to each processor 
	displs: integer array (of length group size). Entry i specifies the displacement (relative to sendbuf from which to take the outgoing data to process i 
	sendtype: data type of send buffer elements (handle) 
	recvbuf: address of receive buffer (choice, significant only at processes not root) 
	recvcount: number of elements in receive buffer (integer) 
	recvtype: data type of receive buffer elements (handle) 
	root: rank of sending process (integer) 
	comm: communicator (handle) 
	*/
	MPI_Scatterv(&valors, num_elements, offsets, MPI_INT, &valors2, num_elements[id], MPI_INT, 0, MPI_COMM_WORLD);
	printf("\n%d Scatter Done", id);
	qs(&valors2[0], num_elements[id]);
	printf("\n%d qs Done", id);
	vout = valors2;
	vin = valors;
	vtmp = valors3;
	int num_received_values;
	int counter = 0;
	for (i = 2; i <= ideal_num_processos; i *= 2)
	{
		if (id % i > 0)  // envies
		{
			proces_font = id;
			proces_desti = id / i * i;
			printf("\nProces %d en iteracio %d envia %d dades a %d", id, counter, num_elements[id], proces_desti);
			printf("\n");
			MPI_Send (vout, num_elements[id], MPI_INT, proces_desti, 0, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			//return 0;
			// fin de hebra
		}
		else  // reps
		{
			proces_font = id / i * i + i / 2;
			proces_desti = id;
			if (proces_font != id && proces_font < parts)
			{
				printf("\nProces %d en iteracio %d INTENTA rebre dades de %d", id, counter, proces_font);
				MPI_Recv(vin, NN, MPI_INT, proces_font, 0, MPI_COMM_WORLD, &estat);
				MPI_Get_count(&estat, MPI_INT, &num_received_values);
				printf("\nProces %d en iteracio %d rep %d dades de %d", id, i / 2, num_received_values, proces_font);
				num_elements[id] = merge2_different(vout, num_elements[id], vin, num_received_values, vtmp);
				vin = vtmp;
				vtmp = vout;
				vout = vin;
			}
		}
		counter++;
  	}

	//if (id == 5) for (i = 0; i < num_elements[id]; i++) printf("%d ", valors2[i]);

    MPI_Finalize();

	// Validacio
	bool correct = 1;
	for (i = 1; i < NN; i++) 
		correct &= vout[i - 1] <= vin[i];

	for (i = 0; i < NN; i += 100)
		sum += vout[i];

	printf("validacio %lld \n",sum);

    free(num_elements);
    free(offsets);

	return 0;
}
/*

	// Merge en arbre
	vin = valors;
	vout = valors2;
	for (m = 2 * porcio; m <= ndades; m *= 2)
	{
		for (i = 0; i < ndades; i += m)
			merge2(&vin[i], m, &vout[i]);
		vtmp = vin;
		vin = vout;
    	vout = vtmp;
  	}

	// Validacio
	for (i = 1; i < ndades; i++) 
		assert(vin[i - 1] <= vin[i]);

	for (i = 0; i < ndades; i += 100)
		sum += vin[i];

	printf("validacio %lld \n",sum);
	exit(0);
}
*/