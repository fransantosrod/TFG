/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 25-02-2017
Descripción: Fichero que contiene las 
implementaciones del tercer módulo encargado
de detectar la suplantación de SSID
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_aux.h"
#include "func_aux_mod3.h"


int main () {

	/*-------------------------------------------
		Tareas:
			0. Comprobar si el fichero existe
			1. Iniciar airodump para capturar y alamcenarlo
				en un fichero
			2. Esperar un tiempo para analizar
			bien el canal
			3. Para la captura 
			X4. Leer el fichero
			5. Analizar la lectura
			6. Tomar acciones si hay alguna coincidencia
	-------------------------------------------*/
	//Estructura que almacenará el contenido del fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los SSID leidos
	struct INFO_SSID info_ssid;

	//Leemos el fichero
	contenido_del_fichero = lee_fichero_csv("primera_captura_prueba.csv");

	info_ssid = procesa_fichero_CSV(contenido_del_fichero);
	
	int i =0;
	for (i=0; i<info_ssid.num_ssid;i++){
		printf("%s %s\n", info_ssid.essid[i], info_ssid.bssid[i]);
	}

	return 0;
}
