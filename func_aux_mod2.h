/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 21-02-2017
Descripción: Fichero que contiene las 
declaraciones y constantes de las 
funciones auxiliares que se usarán en el 
segundo módulo
-------------------------------------------*/
#include "constantes_y_librerias.h"
#include "func_aux.h"

#ifndef func_aux_mod2
#define func_aux_mod2

/*----------------------------------------------------------
	Función que se encarga de leer un fichero y buscar 
	la palabra deseada para almacenar información 

	Recibe: Una estructura del tipo CONTENIDO_FICHERO y 
	la línea en la que debe empezar a buscar

	Devuelve: Estructura del tipo ESTRUCTURA_REGLA
----------------------------------------------------------*/
struct ESTRUCTURA_REGLA busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero, int linea_inicio);


/*-------------------------------------------------------------
	Función que se encarga de registrar las reglas ante los 
	ataques MITM que se puedan producir para posteriormente
	poder borrarlas ya que estas serán temporales

	Recibe: Estructura del tipo ESTRUCTURA_REGLA con la info
	de las relglas que queremos crear

	Devuelve: Nada
------------------------------------------------------------*/
void registra_Regla(struct ESTRUCTURA_REGLA informacion_regla);


/*--------------------------------------------------------
	Función que se encarga de detectar cuando una regla
	sobrepasa un tiempo dado, en este caso una hora

	Recibe: El nombre del fichero

	Devuelve: Nada
-------------------------------------------------------*/
void detecta_Registro_caducado(char *nombre_fichero);


/*-----------------------------------------------------
	Función que se encarga de eliminar una línea del
	fichero de registro de reglas

	Recibe: El nombre del fichero de registro y la
	línea que se desea eliminar

	Devuelve: Nada
----------------------------------------------------*/
void elimina_Registro(char *nombre_fichero, int numero_linea);



/*-------------------------------------------------
	Función que se encarga de localizar en el 
	fichero de reglas creadas para los ataques 
	MITM si existen coincidencias ante una regla
	en concreto

	Recibe: El nombre del fichero donde están
	almacenados los registros, una estructura 
	del tipo ESTRUCTURA_REGLA y la posición 
	dentro de esta estructura de la regla 
	que se quiere buscar

	Devuelve: Un entero indicando la línea en
	la que encontró la coincidencia o "-1" si
	no encontrnó ninguna
-------------------------------------------------*/
int busca_Registro(char * fichero_registro, struct ESTRUCTURA_REGLA informacion_regla, int pos_estructura);
#endif