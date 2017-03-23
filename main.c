/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 10-03-2017
Descripción: Fichero principal que integra
todos los módulos
-------------------------------------------*/

#include "constantes_y_librerias.h"
#include "func_aux.h"

//Constante para definir el número de módulos creados
#define NUM_MODULOS 4

int main () {

	//Variables para la creación de hilos
	pthread_attr_t attr;
	pthread_t thid[NUM_MODULOS];
	
	//Creamos los semáforos
	crea_semaforo(SEM_LECTURA);
	crea_semaforo(SEM_SNORT);
	crea_semaforo(SEM_SID);
	crea_semaforo(SEM_REGLAS_SNORT);

	//Iniciamos los atributos de los hilos
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*-------------------------------------
		Creamos los hilos independientes
		de esta forma cada función es un
		proceso independiente
	--------------------------------------*/
	pthread_create(&thid[0], &attr, mod1, NULL);
	pthread_create(&thid[1], &attr, mod2, NULL);
	pthread_create(&thid[2], &attr, mod3, NULL);
	pthread_create(&thid[3], &attr, mod4, NULL);

	pthread_attr_destroy (&attr);
	
	//Dejamos en una espera continua al proceso principal
	while(true)
		sleep(INTERVALO_LECTURA);

	//Eliminamos los semáforos
	/*-----------------------------------------
		Se ha creado el fichero eliminaSem.c
		para eliminar los semáforos cuando
		se detiene el proceso principal y
		los semáforos siguen abiertos
	-----------------------------------------*/
	elimina_semaforo(SEM_LECTURA);
	elimina_semaforo(SEM_SNORT);
	elimina_semaforo(SEM_SID);
	elimina_semaforo(SEM_REGLAS_SNORT);

	return 0;
}