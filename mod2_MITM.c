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
#include "func_aux.h"
#include "func_aux_mod2.h"



int main () {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variable donde almacenamos el fichero que queremos leer
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para conocer el número de línea que leimos anteriormente
	int num_lineas_anterior;
	//Variable auxiliar para almacenar el nombre del fichero que se crea
	char *nuevo_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Inicalizamos las variables
	strcpy(nombre_fichero, "arpwatch_prueba.log");
	num_lineas_anterior = INICIO;

	//Leemos el fichero
	contenido_del_fichero = lee_fichero(nombre_fichero);
	
	//Comprobamos si el número de líneas leidas está cerca del máximo (NUM_FRASES)
	if ( contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){
		
		//Comprobamos si hemos leido alguna línea nueva
		if (contenido_del_fichero.num_frases_fichero > num_lineas_anterior){
		
			busca_CAMBIO_EN_MAC(contenido_del_fichero);
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
			busca_CAMBIO_EN_MAC(contenido_del_fichero);
			//crea_y_escribe_regla("local.rules_prueba", contenido_fichero_alerta, "drop");
			contenido_del_fichero.num_frases_fichero = INICIO;
		}
	}
	
	num_lineas_anterior = contenido_del_fichero.num_frases_fichero;

	//Liberamos memoria 
	free(nombre_fichero);
	free(nuevo_fichero);
	
	return 0;
}

