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

scp -P 8888 nomfitxer 39932490-M@localhost:.

Nota: Tots els 8888 corresponen a un port arbitrari




