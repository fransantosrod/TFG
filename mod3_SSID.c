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
#include <unistd.h>
#include "func_aux.h"
#include "func_aux_mod3.h"

#define ESSID_propio "M"
#define BSSID_propio "D"


int main () {

	//Estructura que almacenará el contenido del fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los SSID leidos
	struct INFO_SSID info_ssid;
	//Estructura que almacenará los SSID coincidentes
	struct INFO_SSID ssid_coincidentes;
	//Variable para almacenar el nombre del fichero
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//String para almacenar los comandos auxiliares
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);

	//Inicializamos
	strcpy(nombre_fichero, "primera_captura_prueba.csv");

	while(true){
		//Escanemaos la red WiFi
		//DESCOMENTAR
		//escanea_WiFi();

		//Leemos el fichero
		contenido_del_fichero = lee_fichero_csv(nombre_fichero);

		//Obtenemos los SSID leidos
		info_ssid = procesa_fichero_CSV(contenido_del_fichero);
		
		//Buscamos posibles SSID duplicados
		ssid_coincidentes = busca_SSID(info_ssid, ESSID_propio, BSSID_propio);

		//DESCOMENTAR CUANDO ESTÉ EL CÓDIGO EN LA RASPBERRY YA QUE CREA UNA REGLA DE IPTABLES
	/*	
		//Creamos las reglas para esos SSIDs que hemos detectado
		bloquea_SSID(ssid_coincidentes);
		
		//Eliminamos el fichero para la próxima captura
		strcpy(comando, "sudo rm -rf ");
		strcat(comando, nombre_fichero);
		strcat(comando, ".*");
		system(comando);
		
	*/
		sleep(INTERVALO_LECTURA);
	}
	//Liberamos memoria
	free(nombre_fichero);
	free(comando);
	
	return 0;
}

