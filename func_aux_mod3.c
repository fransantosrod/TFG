/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 27-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usarán en el tercer módulo
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "func_aux.h"
#include "func_aux_mod3.h"


/*------------------------------------------------------------
	Función que se encarga de leer un fichero de tipo CSV

	Recibe: El nombre del fichero a leer

	Devuelve: Una estructura del tipo CONTENIDO_FICHERO 
				con los datos obtenidos del fichero
------------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero){
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//String para almacenar la palabra antes de introducirla en la estructura
	char *palabra_aux = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable para contabilizar el número de palabras
	int palabras;
	//Variables para contabilizar el número de frases
	int frases;
	//Variable auxiliar para recorrer el string
	int letra;
	//Variable para el fichero que vamos a leer
	FILE *fichero_lectura;
	//Variable para recorrer el fichero caracter a caracter
	signed char c;
	//Variable para almacenar el caracter que se ha leido anteriormente
	signed char c_ant;


	//Abrimos el fichero
	fichero_lectura = fopen(fichero, "r");

	//Inicializamos
	c_ant = ESPACIO;
	palabras = INICIO;
	frases = INICIO;
	letra = INICIO;


	if (fichero_lectura != NULL){

		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF){
			
			//Comprobamos que no superamos el límite
			if (frases < NUM_FRASES){

				// Si estamos ante una línea en blanco
				if ( (c == SALTO_DE_LINEA && c_ant == SALTO_DE_LINEA)){
					
					//Estamos ante una línea vacía
					//Reiniciamos el contador de palabras y el de letras
					palabras = INICIO;
					letra= INICIO;
				}
				
				//Si no estamos ante una línea en blanco
				else  {	

					//Comprobamos que no estamos ante un doble espacio en blanco
					if ( !(c  == ESPACIO && c_ant == ESPACIO) ){
						
						//Nos aseguramos que no es el caracter ',' que se usa para separar palabras
						if (c != COMA){

							//Comprobamos si estamos ante un caracter que no sea '\n' ni ' '
							if (c != SALTO_DE_LINEA && c != ESPACIO ){
								
								//Almacenamos ese caracter	
								palabra_aux[letra] = c;
								//Aumentamos el contador para almacenar el próximo caracter en esa posición
								letra++;

							}
							
							//Si estamos ante un salto de línea
							else if (c == SALTO_DE_LINEA ) {
								
								//Añadimos el terminador
								palabra_aux[letra++] = TERMINADOR;
								
								//Almacenamos la palabra en la estructura
								contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);
								//Almacenamos el número de palabras que tiene esa frase
								contenido_del_fichero.palabras_por_frase[frases] = palabras;
								
								//Aumentamos el número de frases
								frases++;
								
								//Reiniciamos los contadores
								palabras = INICIO;
								letra = INICIO;
						
							}
							
							//Si estamos ante un espcacio
							else if (c == ESPACIO){
								
								//Añadimos el terminador
								palabra_aux[letra++] = TERMINADOR;

								//Almacenamos la palabra en la estructura
								contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);

								//Reiniciamos los contadores
								palabras++;
								letra = INICIO;
							
							}
						}
					}
				}
			
				//Guardamos el caracter que acabamos de leer
				c_ant = c;
			}
			//En caso contrario indicamos que hemos sobrepasado el número de líneas
			else {

				fprintf(stderr, "El fichero %s supera el número de líneas\n", fichero);
			}
		}

		//Almacenamos el número total de líneas leidas
		contenido_del_fichero.num_frases_fichero = frases;

	}

	else {
	
		fprintf(stderr,"Error al abrir el archivo\n");
	
	}

	//Cerramos el fichero
	fclose(fichero_lectura);
	
	//Liberamos la memoria
	free(palabra_aux);

	return contenido_del_fichero;
}