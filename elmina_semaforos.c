/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 13-03-2017
Descripción: Fichero que elimina los semáforos
creados en el programa
-------------------------------------------*/

#include "constantes_y_librerias.h"
#include "func_aux.h"

int main () {

//Eliminamos los semáforos
	elimina_semaforo(SEM_LECTURA);
	elimina_semaforo(SEM_SNORT);
	elimina_semaforo(SEM_SID);
	elimina_semaforo(SEM_REGLAS_SNORT);
	
}