# CPM

## Per connectar-se a zoo desde fora de la URV

Establim el túnel mantenint un terminal obert amb aquesta instrucció

```
ssh -NfL 8888:zoo:22 64654638-M@portal1-deim.urv.cat
```
I ens connectem utilitzant un altre terminal amb la següent comanda:
```
ssh -X -p 8888 64654638-M@localhost
```

## Per connectar-se desde dins de la URV

Ens connectem directament per ssh:
```
ssh 64654638-M@portal.lab.deim
```

## Per a copiar fitxers
Copiem utilitzant la copia remota
```
scp -P 8888 nomfitxer 64654638-M@localhost:.
```
Nota: Tots els 8888 corresponen a un port arbitrari
 
o bé clonem el repositori (compte, si fas això i els teus companys fan servir vim per editar projectes de 100 KLOCs et podries convertir en hacker-man)

##### OpenMP
## Per a compilar
Utilitzem el makefile situannos a la carpeta src
```
make QS_Paralel
```

## Per executar
per a executar al servidor zoo ho fem tal cual. Podem fer servir la 
execucio definida al makefile per a mesurar el temps
```
make time_P
```

Per a executar a gat hem de fer servir la comanda d'execucio remota.
```
srun -p gat time ./QS_Paralel 300000000 32
```

##### MPI

A la pràctica 2.1, amb 250000000 enters obtenim un número de validació de 2684311754311241.

## Per a instalar el compilador i les llibreries
```
sudo apt install libopenmpi-dev
```

## Per a compilar
```
mpicc -O3 -o output QS_Secuencial.c
```

## Per a executar
```
mpirun -np num_processos output
```
A on num_processos es un enter que especifica el nombre de processos a crear.

## Per a executar en diversos cores de la màquina pop
```
salloc -p pop -n 14 srun -n 14 mpirun -n 14 out.bin
```




