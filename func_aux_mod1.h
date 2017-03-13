/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 13-03-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares del primer módulo
-------------------------------------------*/
#include "constantes_y_librerias.h"
#include "func_aux.h"


#ifndef func_aux_mod1
#define func_aux_mod1

/* -------------------------------------------------------
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	el nombre que vamos a buscar y la línea por la
	que debe empezar a buscar
	Devuelve: Estructura del tipo ESTRUCTURA_REGLA
----------------------------------------------------------*/
struct ESTRUCTURA_REGLA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, 
	char * nombre_Coincidencia, int linea_inicio);

/* -------------------------------------------------------
	Función que se encarga de determinar si una IP pertenece
	a una red local o no

	Recibe: Un string que contiene la dirección IP
	Devuelve: Un booleano indicando si pertenece a la red local
----------------------------------------------------------*/
bool comprueba_IP(char *direccion_IP);




#endif