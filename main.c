/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 10-03-2017
Descripción: Fichero principal que integra
todos los módulos
-------------------------------------------*/

#include "func_aux.h"

int main () {

	/*--------------------------
		DEBE EJECUTARSE COMO
		SUPERUSUARIO
	--------------------------*/
	//Creamos los semáforos
	crea_semaforo(SEM_LECTURA);
	crea_semaforo(SEM_SNORT);
	crea_semaforo(SEM_SID);
	crea_semaforo(SEM_REGLAS_SNORT);

	//Invocamos el primer módulo
	mod1();

	//Eliminamos los semáforos
	elimina_semaforo(SEM_LECTURA);
	elimina_semaforo(SEM_SNORT);
	elimina_semaforo(SEM_SID);
	elimina_semaforo(SEM_REGLAS_SNORT);
}