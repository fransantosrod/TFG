/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 9-03-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes del 
cuarto módulo
-------------------------------------------*/
#include "constantes_y_librerias.h"
#include "func_aux.h"


#ifndef func_aux_mod4
#define func_aux_mod4

/*-------------------------------------------------------
	Función que se encarga de introducir los valores
	deseados en la base de datos

	Recibe: Estructura del tipo CONTENIDO_FICHERO y
		un entero indicando la posición en la 
		que se debe comenzar a recorrer la estructura
	Devuelve: Nada
-------------------------------------------------------*/
void inserta_en_BBDD(struct CONTENIDO_FICHERO contenido_del_fichero, int frase_inicio);


void *mod4 ();
#endif