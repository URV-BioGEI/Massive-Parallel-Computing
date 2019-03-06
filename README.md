# CPM

#per establir el tunel


ssh -NfL XXXX:zoo:22 loginURV@portal1-deim.urv.cat 

#on XXXX es un numero de port arbitrari ex: 8055

#on loginURV es el login que teniu de la URV (el de llegir el e-mail)



#per connectar-se:
ssh -X -p XXXX loginAP@localhost

#on XXXX es port d'abans (mateix numero)

#on loginAP es le login de la maquina zoo: apXX



#per copiar:

#scp -P XXXX fitxer loginAP@localhost:.

#fitxer es el nom del fitxer a transferir

#ho deixa al home directori de xoo


ssh 39932490-M@portal.lab.deim