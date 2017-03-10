/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 21-02-2017
Descripción: Fichero que contiene las 
declaraciones y constantes de las 
funciones auxiliares que se usarán en el 
segundo módulo
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "func_aux.h"

#ifndef func_aux_mod2
#define func_aux_mod2

//Indica que la dirección MAC ha cambiado para una misma IP
#define CAMBIO_EN_MAC "changed"
//Indica que la MAC ha cambiado en un intervalo muy corto de tiempo 
#define CAMBIO_MAC_INMEDIATO "flip"
//Posición que ocupa la dirección IP
#define POS_DIR_IP 3
#define POS_DIR_IP_INM POS_DIR_IP-1
//Posición que ocupa la nueva dirección MAC
#define POS_DIR_MAC_NUEVA 4
#define POS_DIR_MAC_NUEVA_INM POS_DIR_MAC_NUEVA-1
//Posición que ocupa la vieja dirección MAC
#define POS_DIR_MAC_VIEJA 5
#define POS_DIR_MAC_VIEJA_INM POS_DIR_MAC_VIEJA-1
//Tamaño de la dirección MAC
#define TAM_MAC 17
#define IP "IP"
//Constante donde definimos la posición de la hora en el fichero de registro
#define HORA 0
//Constante paara definir el paso de una hora
#define PASA_UNA_HORA 100
//Constante para definir la posición de la bandera que indica si la IP es origen o destino
#define POS_BANDERA_IP 4
//Constante para definir la posición de la bandera que indica si la IP lleva puerto asociado
#define POS_BANDERA_PUERTO 5

/*----------------------------------------------------------
	Función que se encarga de leer un fichero y buscar 
	la palabra deseada para almacenar información 

	Recibe: Una estructura del tipo CONTENIDO_FICHERO y 
	la línea en la que debe empezar a buscar

	Devuelve: Nada
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



/*-------------------------------------------
	Función que se encarga de implementar
	las funcionalidades del módulos 2

	Recibe: Nada
	Devuelve: Nada
-------------------------------------------*/
void *mod2();
#endif