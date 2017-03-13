#Makefile WIPS

WIPS: main.o mod1_DoS.o func_aux_mod1.o mod2_MITM.o func_aux_mod2.o mod3_SSID.o mod3_SSID.o func_aux_mod3.o mod4_database.o func_aux_mod4.o func_aux.o
	gcc -o WIPS main.o mod1_DoS.o func_aux_mod1.o mod2_MITM.o func_aux_mod2.o mod3_SSID.o func_aux_mod3.o mod4_database.o func_aux_mod4.o func_aux.o -pthread

main.o: main.c constantes_y_librerias.h 
	gcc -W -Wall -c main.c

mod4_database.o: mod4_database.c func_aux_mod4.h
	gcc -W -Wall -c mod4_database.c 

func_aux_mod4.o: func_aux_mod4.c func_aux_mod4.h
	gcc -W -Wall -c func_aux_mod4.c

mod3_SSID.o: mod3_SSID.c func_aux_mod3.h
	gcc -W -Wall -c mod3_SSID.c

func_aux_mod3.o: func_aux_mod3.c func_aux_mod3.h
	gcc -W -Wall -c func_aux_mod3.c

mod2_MITM.o: mod2_MITM.c func_aux_mod2.h
	gcc -W -Wall -c mod2_MITM.c

func_aux_mod2.o: func_aux_mod2.c func_aux_mod2.h
	gcc -W -Wall -c func_aux_mod2.c

mod1_DoS.o: mod1_DoS.c func_aux_mod1.h
	gcc -W -Wall -c mod1_DoS.c

func_aux_mod1.o: func_aux_mod1.c func_aux_mod1.h
	gcc -W -Wall -c func_aux_mod1.c

func_aux.o: func_aux.c func_aux.h
	gcc -W -Wall -c func_aux.c

eliminaSem: elmina_semaforos.c constantes_y_librerias.h func_aux.c func_aux.h
	gcc -W -Wall -o eliminaSem elmina_semaforos.c func_aux.c -pthread

clean:
	rm *.o