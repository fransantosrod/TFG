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

#define COMA ','

struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero);

int main () {

	/*-------------------------------------------
		Tareas:
			0. Comprobar si el fichero existe
			1. Iniciar airodump para capturar y alamcenarlo
				en un fichero
			2. Esperar un tiempo para analizar
			bien el canal
			3. Para la captura 
			4. Leer el fichero
			5. Analizar la lectura
			6. Tomar acciones si hay alguna coincidencia
	-------------------------------------------*/
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;
	
	contenido_del_fichero = lee_fichero_csv("primera_captura_prueba.csv");


	int i = 0;
	int j=0;

	for (i=0; i< contenido_del_fichero.num_frases_fichero; i++){
		
		for (j=0; j< contenido_del_fichero.palabras_por_frase[i]; j++){
			printf("%s ", contenido_del_fichero.contenido_leido_del_fichero[i][j]);
		}
		printf("\n");
	}


	return 0;
}

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
			
			// Si estamos ante una línea en blanco
			if ( (c == SALTO_DE_LINEA && c_ant == SALTO_DE_LINEA)){
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
							
							palabra_aux[letra] = c;
							letra++;
							printf("%c", c);
						}
						
						//Si estamos ante un salto de línea
						else if (c == SALTO_DE_LINEA ) {
						
							palabra_aux[letra++] = TERMINADOR;
							contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);
							contenido_del_fichero.palabras_por_frase[frases] = palabras;
							frases++;
							palabras = INICIO;
							letra = INICIO;
							printf("\n");
					
						}
						
						//Si estamos ante un espcacio
						else if (c == ESPACIO){
							
							palabra_aux[letra++] = TERMINADOR;
							contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);

							palabras++;
							letra = INICIO;

							printf(" ");
						
						}
					}
				}
			}

			c_ant = c;
		}

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