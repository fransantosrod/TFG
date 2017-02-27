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
//Constante para definir la palabra ESSID
#define ESSID "ESSID"
//Connstante para definir la palabra BSSID
#define BSSID "BSSID"
//Constante para definir la palabra que indica que comienza la info de los clientes
#define INFO_CLIENTES "Station MAC"
//Constante para definir el número máximo de SSID que vamos a almacenar en la estructura
#define NUM_SSID 50

//Estructura para almacenar los SSID leidos
typedef struct INFO_SSID {
	
	//Array para almacenar los SSID que se lean
	char *essid[NUM_SSID];
	//Array para almacenar los BSSID que se lean
	char *bssid[NUM_SSID];
	//Variable para almacenar la cantidad de SSID leidos
	int num_ssid;

}info_ssid;

/*------------------------------------------------------------
	Función que se encarga de leer un fichero de tipo CSV

	Recibe: El nombre del fichero a leer

	Devuelve: Una estructura del tipo CONTENIDO_FICHERO 
				con los datos obtenidos del fichero
------------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero);

/*------------------------------------------------------
	Función que se encarga de extraer del fichero leido
	la información de los SSID

	Devuelve: Estructura del tipo INFO_SSID formada por:
	Recibe: Estructura del tipo CONTENIDO_FICHERO
------------------------------------------------------*/
struct INFO_SSID procesa_fichero_CSV(struct CONTENIDO_FICHERO contenido_del_fichero);
#endif