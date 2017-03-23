#Maefile interfaz Web

all: borraRegla.o procesa_borrado_regla.o muestraReglasIptables.o muestraRegistroReglasMITM.o muestraReglasSnort.o

borraRegla.o: borraRegla.c /home/salas/TFG/func_aux.c /home/salas/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o borraRegla.out borraRegla.c /home/salas/TFG/func_aux.c -pthread

procesa_borrado_regla.o: procesa_borrado_regla.c /home/salas/TFG/func_aux.c /home/salas/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o procesa_borrado_regla.out procesa_borrado_regla.c /home/salas/TFG/func_aux.c -pthread

muestraReglasIptables.o: muestraReglasIptables.c /home/salas/TFG/func_aux.c /home/salas/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o muestraReglasIptables.out muestraReglasIptables.c /home/salas/TFG/func_aux.c -pthread

muestraRegistroReglasMITM.o: muestraRegistroReglasMITM.c /home/salas/TFG/func_aux.c /home/salas/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o muestraRegistroReglasMITM.out muestraRegistroReglasMITM.c /home/salas/TFG/func_aux.c -pthread

muestraReglasSnort.o: muestraReglasSnort.c /home/salas/TFG/func_aux.c /home/salas/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o muestraReglasSnort.out muestraReglasSnort.c /home/salas/TFG/func_aux.c -pthread