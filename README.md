# CPM

## Per connectar-se a zoo desde fora de la URV

Establim el túnel mantenint un terminal obert amb aquesta instrucció

'''
ssh -NfL 8888:zoo:22 39932490-M@portal1-deim.urv.cat
'''
I ens connectem utilitzant un altre terminal amb la següent comanda:
'''
ssh -X -p 8888 39932490-M@localhost
''' 

## Per connectar-se desde dins de la URV

Ens connectem directament per ssh:
'''
ssh 39932490-M@portal.lab.deim
'''

## Per a copiar fitxers
Copiem utilitzant la copia remota
'''
scp -P 8888 nomfitxer 39932490-M@localhost:.
'''
Nota: Tots els 8888 corresponen a un port arbitrari
 
o bé clonem el repositori (let's be intelligent all together)

##### OpenMP
## Per a compilar
Utilitzem el makefile situannos a la carpeta src
'''
make QS_Paralel
'''

## Per executar
per a executar al servidor zoo ho fem tal cual. Podem fer servir la 
execucio definida al makefile per a mesurar el temps
'''
make time_P
'''

Per a executar a gat hem de fer servir la comanda d'execucio remota.
'''
srun -p gat time ./QS_Paralel 300000000 32
'''

##### MPI
## Per a instalar el compilador i les llibreries
```
sudo apt install libopenmpi-dev
```

## Per a compilar
```
mpicc -o output QS_Secuencial.c
```

## Per a executar
```
mpirun -np num_processos output
```
A on num_processos es un enter que especifica el nombre de processos a crear.




