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
#include "func_aux.h"

#define CAMBIO_EN_MAC "changed"
#define POS_DIR_IP 3
#define POS_DIR_MAC_NUEVA 4
#define POS_DIR_MAC_VIEJA 5


void busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero);

int main () {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variable donde almacenamos el fichero que queremos leer
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);

	//Inicalizamos las variables
	strcpy(nombre_fichero, "arpwatch_prueba.log");
	

	//Leemos el fichero
	contenido_del_fichero = lee_fichero(nombre_fichero);

	busca_CAMBIO_EN_MAC(contenido_del_fichero);

	return 0;
}

/*----------------------------------------------------------
	Función que se encarga de leer un fichero y buscar 
	la palabra deseada para almacenar información 

	Recibe: Una estructura del tipo CONTENIDO_FICHERO

	Devuelve: Nada

	Caracteristicas:
		Función pensada para leer el fichero de la
		herramienta arpwatch para buscar en el posibles
		conflictos de direcciones cuando se está realizando
		un ataque MITM
----------------------------------------------------------*/

void busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero) {

	//Variable auxiliar para recorrer las frases leidas
	int cont_aux_frases;
	//Variable auxiliar para recorrer las palabras de cada frase
	int cont_aux_palabras;
	//Variable para almacenar la linea en la que hemos detectado la coincidencia
	int linea_coincidencia;
	//Variable para almacenar el número dentro del array en el que detectamos la coincidencia
	int palabra_coincidencia;
	//Bandera para indicar si hemos detectado una coincidencia
	bool flag;


	//Inicializamos
	cont_aux_frases = INICIO;
	cont_aux_palabras = INICIO;
	linea_coincidencia = INICIO;
	palabra_coincidencia= INICIO;
	flag = false;

	//Bucle para recorrer las frases que hemos leido del fichero
	for (cont_aux_frases=0; 
		cont_aux_frases < contenido_del_fichero.num_frases_fichero; 
		cont_aux_frases++){
		
		/*------------------------------------------------------
			En el fichero arpwatch.log dólo vamos a encontrar
			información de la herramienta arpwatch y cada 
			notificación se hará en una única línea
			de ahí que cada línea sea una notificación distinta
		------------------------------------------------------*/
		//Comprobamos si hemos encontrado la palabra que indica el cambio de MAC y estamos en la siguiente línea
		if (cont_aux_frases == linea_coincidencia+SIGUIENTE_LINEA && flag ==true){
			//Cambiamos el valor de la bandera para señalizar el instante en el que encontramos otra
			flag = false;

		}

		//Recorremos las paralbras que hemos leido dentro de cada frase
		for(cont_aux_palabras=0; 
			cont_aux_palabras <= contenido_del_fichero.palabras_por_frase[cont_aux_frases];
			cont_aux_palabras++) {

			//Comprobamos si la palabra actual es la que estamos buscando y que nos indica el cambio de MAC para una misma dir_IP 
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras],
				CAMBIO_EN_MAC) == IGUAL){

				//Almacenamos la línea en la que hemos encontrado la coincidencia
				linea_coincidencia = cont_aux_frases;
				//Almacenamos la palabra en la que hemos encontrado la coincidencia
				palabra_coincidencia = cont_aux_palabras;
				//Cambiamos el valor de la bandera para indicar que hemos encontrado una coincidencia
				flag = true;
			}

			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección IP
			if ( flag == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_IP){
				printf("Dirección IP conflicto: %s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
			}

			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC nueva
			else if (flag == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_NUEVA){
				printf("	Dirección MAC nueva: %s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
			}
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC antigua
			else if (flag == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_VIEJA){
				printf("	Dirección MAC vieja: %s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
			}
			
		}
	}

}