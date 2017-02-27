/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 27-02-2017
Descripción: Fichero que contiene las 
declaraciones y constantes de las 
funciones auxiliares que se usarán en el 
tercer módulo
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "func_aux.h"

#ifndef func_aux_mod3
#define func_aux_mod3s

//Constante para definir el caracter ','
#define COMA ','

/*------------------------------------------------------------
	Función que se encarga de leer un fichero de tipo CSV

	Recibe: El nombre del fichero a leer

	Devuelve: Una estructura del tipo CONTENIDO_FICHERO 
				con los datos obtenidos del fichero
------------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero);

#endif