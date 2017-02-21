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

void elimina_Regla(char *nombre_fichero_borrar, struct ESTRUCTURA_REGLA informacion_regla, 
	int pos_dentro_estruct_regla);

int main () {

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

	//Leemos el fichero
	contenido_del_fichero = lee_fichero(nombre_fichero);
	
	//Comprobamos si el número de líneas leidas está cerca del máximo (NUM_FRASES)
	if ( contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){
		
		//Comprobamos si hemos leido alguna línea nueva
		if (contenido_del_fichero.num_frases_fichero > num_lineas_anterior){
			
			//Buscamos si hemos detectado un cambio en la MAC
			informacion_regla=busca_CAMBIO_EN_MAC(contenido_del_fichero);
			
			//Comprobamos si tenemos que crear alguna regla
			if (informacion_regla.numero_lineas > INICIO){

				//Creamos la regla
				//crear_regla = crea_y_escribe_regla("local.rules_prueba", informacion_regla, "MITM");
				//Comprobamos si la hemos creado correctamente
				if (crear_regla == true) {
					//Cambiamos el valor de la bandera para que solo reiniciemos cuando se crea una regla nueva
					crear_regla = false;
					//Reiniciamos Snort
					//recarga_Snort();
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
			informacion_regla = busca_CAMBIO_EN_MAC(contenido_del_fichero);
			
			//Comprobamos si hemos detectado la necesidad de crear alguna regla
			if (informacion_regla.numero_lineas > INICIO){
				
				//Creamos la regla
				//crear_regla = crea_y_escribe_regla("local.rules_prueba", informacion_regla, "MITM");

				//Comprobamos que la hemos creado correctamente
				if (crear_regla == true){

					//Cambiamos el valor de la bandera
					crear_regla = false;
					//Recargamos Snort
					//recarga_Snort();
				}
			}
			
		}
		contenido_del_fichero.num_frases_fichero = INICIO;
	}
	
	num_lineas_anterior = contenido_del_fichero.num_frases_fichero;

	//Liberamos memoria 
	free(nombre_fichero);
	free(nuevo_fichero);
	
	return 0;
}

/*---------------------------------------------------------
|
|	FUNCIONES QUE QUEDAN POR HACER;
|
|	1. REGISTRAR LAS REGLAS QUE SE CREAN CON SU TIEMPO PARA SABER CUANDO BORRARLAS
|	2. DETECTAR CUAL DE LAS REGLAS CREADAS HA SOBREPASADO EL TIEMPO DE EXISTENCIA
|	3. ELIMINAR ESA REGLA
|
--------------------------------------------------------*/
/*--------------------------------------------------------
	Función que se encarga de eliminar una regla
	del fichero de reglas de Snort

	Recibe: El fichero del que se quiere eliminar
	las reglas, la estructura del tipo ESTRUCTURA_REGLA
	que contiene la información de la regla que se
	desea borrar y la posición dentro de la 
	estructura

	Devuelve: Nada

	Caracteristicas:
		En primer lugar lee el fichero del que se 
		quieren borrar sus líneas para tener almacenada
		una copia, tras esto se busca la línea donde
		está la regla que queremos elminiar y tras esto
		abre el fichero vacio y lo reescribe con los
		datos que almacenó al principio saltandose la 
		línea que deseamos eliminar
-------------------------------------------------------*/

void elimina_Regla(char *nombre_fichero_borrar,struct ESTRUCTURA_REGLA informacion_regla, 
	int pos_dentro_estruct_regla){

	//Variable para almacenar el fichero de donde vamos a borrar
	FILE *fichero_borrar;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;
	//Variable auxiliar para almacenar la regla que vamos a sobreescribir
	char *regla_aux = (char *)malloc(sizeof(char) *NUM_CARACTERES_REGLAS);
	//Variable para almacenar la línea en la que hemos encontrado la regla
	int linea_regla;

	//Inicializamos las variables
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;
	linea_regla = INICIO;

	//Leemos el fichero del que queremos borrar la regla
	contenido_del_fichero = lee_fichero(nombre_fichero_borrar);	

	//Buscamo dicha regla en el fichero
	linea_regla = busca_Regla(contenido_del_fichero, informacion_regla, pos_dentro_estruct_regla);


	//Si hemos encontrado la regla
	if (linea_regla >= INICIO){

		//Abrimos el fichero vacio para volver a escribirlo
		fichero_borrar = fopen(nombre_fichero_borrar, "w");
		
		//Recorremos el array que rellenamos cuando lo leimos por primera vez
		for (cont_aux_frases_fichero=0;
		cont_aux_frases_fichero < contenido_del_fichero.num_frases_fichero;
		cont_aux_frases_fichero++){
			
			//Comprobamos que la línea que vamos a escribir no es la que queremos eliminar
			if (cont_aux_frases_fichero != linea_regla) {

				for(cont_aux_palabras_fichero =0;
					cont_aux_palabras_fichero <= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero];
					cont_aux_palabras_fichero++) {

					//Si estamos ante la primera palabra de la frase
					if (cont_aux_palabras_fichero == 0){
						//Copiamos para iniciar una nueva regla
						strcpy(regla_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
						strcat(regla_aux, " ");
					}
					//Si no encontramos ante otra palabra
					else {
						//Concatenamos las palabras para seguir creando la regla
						strcat(regla_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
						strcat(regla_aux, " ");
					}

				}
			//Añadimos un salto de línea
			strcat(regla_aux, "\n");
			//Escribimos la regla en el fichero
			fputs(regla_aux, fichero_borrar);
			}
		}
		//Cerramos el fichero
		fclose(fichero_borrar);
	}
	//Liberamos memoria
	free(regla_aux);
}