#Maefile interfaz Web

all: borraRegla.o procesa_borrado_regla.o muestraRegistroReglasMITM.o muestraReglasSnort.o

borraRegla.o: borraRegla.c /home/pi/TFG/func_aux.c /home/pi/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o borraRegla.out borraRegla.c /home/pi/TFG/func_aux.c -pthread

procesa_borrado_regla.o: procesa_borrado_regla.c /home/pi/TFG/func_aux.c /home/pi/TFG/func_aux_mod2.c /home/pi/TFG/func_aux_mod2.h /home/pi/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o procesa_borrado_regla.out procesa_borrado_regla.c /home/pi/TFG/func_aux.c /home/pi/TFG/func_aux_mod2.c -pthread

muestraRegistroReglasMITM.o: muestraRegistroReglasMITM.c /home/pi/TFG/func_aux.c /home/pi/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o muestraRegistroReglasMITM.out muestraRegistroReglasMITM.c /home/pi/TFG/func_aux.c -pthread

muestraReglasSnort.o: muestraReglasSnort.c /home/pi/TFG/func_aux.c /home/pi/TFG/constantes_y_librerias.h
	gcc -g -W -Wall -o muestraReglasSnort.out muestraReglasSnort.c /home/pi/TFG/func_aux.c -pthread