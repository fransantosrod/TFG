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

#define ESSID_propio "M"
#define BSSID_propio "D"

void bloquea_SSID (struct INFO_SSID ssid_coincidentes);


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
			X5. Analizar la lectura
			6. Tomar acciones si hay alguna coincidencia
	-------------------------------------------*/
	//Estructura que almacenará el contenido del fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los SSID leidos
	struct INFO_SSID info_ssid;
	//Estructura que almacenará los SSID coincidentes
	struct INFO_SSID ssid_coincidentes;

	//Leemos el fichero
	contenido_del_fichero = lee_fichero_csv("primera_captura_prueba.csv");

	info_ssid = procesa_fichero_CSV(contenido_del_fichero);
	
	ssid_coincidentes = busca_SSID(info_ssid, ESSID_propio, BSSID_propio);

	bloquea_SSID(ssid_coincidentes);
	return 0;
}

void bloquea_SSID (struct INFO_SSID ssid_coincidentes){

	/*--------------------------------------------
		Debe comprobar si la coincidencia es de 
		BSSID o ESSID
			
			ESSID; Obtener el BSSID correspondiente
			y bloquear el tráfico mediante IPTABLES
			
			BSSID; A partir de la dirección MAC
			obtener la diercción IP y bloquearla
			mediante IPTABLES
	--------------------------------------------*/
	//Variable para recorrer la estructura
	int cont_aux_ssid;
	//String para almacenar el comando que vamos a utilizar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Inicializamos
	cont_aux_ssid = INICIO;

	for (cont_aux_ssid=0;cont_aux_ssid<ssid_coincidentes.num_ssid;cont_aux_ssid++){

	
		//Comprobamos si la coincidencia se ha dado en el ESSID
		if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == false) {

			//Creamos la regla
			strcpy(comando, "sudo iptables ");
			printf("%s\n", comando);
		}

		//En caso contrario la coincidencia será en el BSSID
		else if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == true){
			printf("%s %s\n", ssid_coincidentes.essid[cont_aux_ssid], ssid_coincidentes.bssid[cont_aux_ssid]);
		}
	}

	free(comando);
}