/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 10-02-2017
Descripción: Fichero que contiene la 
implementación del primer módulo centrado 
en los ataques DoS
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "func_aux.h"

#define INTERVALO_LECTURA 3



int main (){

	
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct CONTENIDO_ALERTA contenido_fichero_alerta;
	//Variable donde almacenamos el fichero que queremos leer
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el instante en el que se cambia el fichero
	time_t tiempo_cambio_fichero = time(NULL);
	//Array para almacenar la fecha
	char t_cambio_fichero[TAM_FECHA];
	//Estructura para almacenar la fecha y hora en el momento deseado
	struct tm *tm;
	//Variable para almacenar el comando que vamos a ejecutar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para conocer el número de línea que leimos anteriormente
	int num_lineas_anterior;
	//Variable para conocer si hemos creado alguna regla o no
	bool regla_creada;

	//Inicializamos las variables
	strcpy(nombre_fichero, "/var/log/snort/alert");
	num_lineas_anterior = INICIO;
	regla_creada=false;

	//Bucle para leer periodicamente el fichero de alertas
	while (true){
		
		//Leemos el fichero
		contenido_del_fichero = lee_fichero(nombre_fichero);
	
		//Comprobamos si el número de líneas leidas está cerca del máximo (NUM_FRASES)
		if ( contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){
		
			if (contenido_del_fichero.num_frases_fichero > num_lineas_anterior){
				//Si no lo está comprobamos si hemos encontrado alguna alerta de DoS
				contenido_fichero_alerta = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
				
				//Comprobamos que hemos detectado alguna alerta
				if (contenido_fichero_alerta.numero_lineas > INICIO){	
					
					//Creamos las reglas con las que hayamos encontrado
					regla_creada = crea_y_escribe_regla("/etc/snort/rules/local.rules", contenido_fichero_alerta, "drop");
					
					//Comprobamos si hemos escrito alguna regla	
					if (regla_creada==true){
						regla_creada = false;
						recarga_Snort();
					}
				}
			}
			
		}

		//En el caso en el que estemos cerca del límite
		else {

			
			/*---------------------------------------------------
				Este fichero (mod1_DoS.c) se encarga de leer
				continuamente un fichero que se va rellenando
				dinámicamente, por lo tanto, una vez que detectamos
				que laas líneas escritas están en el límite 
				(NUM_FRASES-30), antes de que el fichero sobrepase
				el límite y perdamos información, realizamos un
				cambio y pasamos a almacenar el fichero bajo un
				nuevo nombre y creamos uno nuevo para que Snort
				pueda seguir notificando sus alertas.
			---------------------------------------------------*/

			//Obtenemos el instante en el que lo hemos detectado
			tm = localtime(&tiempo_cambio_fichero);
			/*-------------------------------------------------
				La estructura con la que se almacena el nuevo
				fichero es la siguiente:
					nombre_fichero-dia-mes-año_hora:min:seg
			-------------------------------------------------*/
			strftime(t_cambio_fichero, sizeof(t_cambio_fichero), "-%d-%m-%Y_%H:%M:%S", tm);
			
			//Almacecenamos el fichero actual en una copia para no perderlo
			strcpy(comando, "cp -f ");
			strcat(comando, nombre_fichero);
			strcat(comando, " ./");
			//La copia estará formada por el nombre y la fecha anteriormente obtenida
			strcat(comando, nombre_fichero);
			strcat(comando, t_cambio_fichero);
			system(comando);
			
			//Eliminamos el fichero que está apunto de sobrecargarse
			strcpy(comando, "rm -f ./");
			strcat(comando, nombre_fichero);
			system(comando);
			
			//Creamos uno nuevo vacío
			strcpy(comando, "touch -f ./");
			strcat(comando, nombre_fichero);
			system(comando);
			
			/*--------------------------------------------------------
				Para asegurarnos que no perdemos información
				en ningún momento, volvemos a leer el fichero
				al que le hemos cambiado el nombre siempre y
				cuando no supere el número máximo (NUM_FRASES)
				
				Esta comprobació la hacemos por si el fichero 
				se encontraba en el rango entre LIMITE y NUM_FRASES
				y haya información que se pueda perder

			--------------------------------------------------------*/
			strcpy(comando, nombre_fichero);
			strcat(comando, t_cambio_fichero);

			contenido_del_fichero = lee_fichero(comando);

			//Comprobamos que ese fichero no ha superado el límite
			if (contenido_del_fichero.num_frases_fichero < NUM_FRASES){
				
				//Si no lo está comprobamos si hemos encontrado alguna alerta de DoS
				contenido_fichero_alerta = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
				//Creamos las reglas con las que hayamos encontrado
				crea_y_escribe_regla("local.rules_prueba", contenido_fichero_alerta, "drop");
				contenido_del_fichero.num_frases_fichero = INICIO;
			}
		}
		num_lineas_anterior = contenido_del_fichero.num_frases_fichero;
		sleep(INTERVALO_LECTURA);
	}	
	//Liberamos la memoria
	free(nombre_fichero);
	free(comando);

	return 0;

} 
