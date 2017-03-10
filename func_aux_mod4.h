/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 9-03-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes del 
cuarto módulo
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "func_aux.h"


#ifndef func_aux_mod4
#define func_aux_mod4


//Constante para definir la contrasenia de la base de datos
#define CONTRASENIA_BBDD "clientes"
//Constante para definir el usuario con el que nos queremos registrar
#define USUARIO_BBDD  "clientes"
//Constante para definir la bbdd a la que deseamos acceder
#define NOMBRE_BBDD "clientes_asociados"
//Constante para definir la tabla a la que vamos a acceder
#define NOMBRE_TABLA_BBDD "datos_clientes"
//Constante para indicar la posición de la fecha en la bbdd
#define FECHA 2

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