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

/*----------------------------------------------------------
	Función que se encarga de leer un fichero y buscar 
	la palabra deseada para almacenar información 

	Recibe: Una estructura del tipo CONTENIDO_FICHERO

	Devuelve: Nada
----------------------------------------------------------*/
struct ESTRUCTURA_REGLA busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero);

#endif