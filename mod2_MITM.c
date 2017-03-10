/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 20-02-2017
Descripción: Fichero que contiene las 
implementaciones del segundo módulo centrado
en los ataques MITM 
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "func_aux.h"
#include "func_aux_mod2.h"

/*-------------------------------------------
	Función que se encarga de implementar
	las funcionalidades del módulos 2

	Recibe: Nada
	Devuelve: Nada
-------------------------------------------*/

void *mod2 () {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las reglas que queremos crear
	struct ESTRUCTURA_REGLA informacion_regla;
	//Variable donde almacenamos el fichero que queremos leer
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para conocer el número de línea que leimos anteriormente
	int num_lineas_anterior;
	//Variable auxiliar para almacenar el nombre del fichero que se crea
	char *nuevo_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Bandera para indicar si se deben crear reglas
	bool crear_regla;

	//Inicalizamos las variables
	strcpy(nombre_fichero, "arpwatch_prueba.log");
	num_lineas_anterior = INICIO;
	crear_regla = false;

	while(true){

		//Leemos el fichero
		contenido_del_fichero = lee_fichero(nombre_fichero);

		//Miramos a ver si algún registro ha pasado el tiempo ya
		detecta_Registro_caducado("registro_reglas_MITM");
				
		//Comprobamos si el número de líneas leidas está cerca del máximo (NUM_FRASES)
		if ( contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){
				
			//Comprobamos si hemos leido alguna línea nueva
			if (contenido_del_fichero.num_frases_fichero > num_lineas_anterior){
					
				//Buscamos si hemos detectado un cambio en la MAC
				informacion_regla=busca_CAMBIO_EN_MAC(contenido_del_fichero, num_lineas_anterior);
					
				//Registramos la regla que vamos a crear
				registra_Regla(informacion_regla);

				//Comprobamos si tenemos que crear alguna regla
				if (informacion_regla.numero_lineas > INICIO){

						
					//Creamos la regla
					crear_regla = crea_y_escribe_regla("local.rules_prueba", informacion_regla, "MITM");
					//Comprobamos si la hemos creado correctamente
					if (crear_regla == true) {
						//Cambiamos el valor de la bandera para que solo reiniciemos cuando se crea una regla nueva
						crear_regla = false;
						/*DESCOMENTAR
						//Reiniciamos Snort
						recarga_Snort();
						*/
					}
				}
			}
		}
			
		//En el caso en el que estemos cerca del límite
		else {
				
			//Llamamos a la función deseada para que nos vacie el fichero
			nuevo_fichero = vacia_fichero(nombre_fichero);
				
			/*--------------------------------------------------------
				Para asegurarnos que no perdemos información
				en ningún momento, volvemos a leer el fichero
				al que le hemos cambiado el nombre siempre y
				cuando no supere el número máximo (NUM_FRASES)
				
				Esta comprobació la hacemos por si el fichero 
				se encontraba en el rango entre LIMITE y NUM_FRASES
				y haya información que se pueda perder
			--------------------------------------------------------*/
				
			contenido_del_fichero = lee_fichero(nuevo_fichero);
			//Comprobamos que ese fichero no ha superado el límite
			if (contenido_del_fichero.num_frases_fichero < NUM_FRASES){
					
				//Creamos las reglas con las que hayamos encontrado
				informacion_regla = busca_CAMBIO_EN_MAC(contenido_del_fichero, num_lineas_anterior);
					
				//Comprobamos si hemos detectado la necesidad de crear alguna regla
				if (informacion_regla.numero_lineas > INICIO){
						
					//Creamos la regla
					crear_regla = crea_y_escribe_regla("local.rules_prueba", informacion_regla, "MITM");

					//Comprobamos que la hemos creado correctamente
					if (crear_regla == true){

						//Cambiamos el valor de la bandera
						crear_regla = false;
						/*DESCOMENTAR
						//Recargamos Snort
						recarga_Snort();
						*/
					}
				}
					
			}
			contenido_del_fichero.num_frases_fichero = INICIO;
		}
			
		num_lineas_anterior = contenido_del_fichero.num_frases_fichero;
		sleep(INTERVALO_LECTURA);
	}

	//Liberamos memoria 
	free(nombre_fichero);
	free(nuevo_fichero);

	pthread_exit(NULL);

}