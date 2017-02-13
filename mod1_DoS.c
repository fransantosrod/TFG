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
#include "func_aux.h"

#define NUM_CARACTERES_REGLAS 500
#define NUM_SID 10

void crea_y_escribe_regla(FILE *fichero_escritura, struct COINCIDENCIA_REGLA coincidencias_en_reglas, char *accion);


int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero_lectura;
	//Variable para el fichero donde deseamos escribir
	FILE *fichero_escritura;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct COINCIDENCIA_REGLA coincidencias_en_reglas;

	//Inicializamos las variables
	fichero_lectura = fopen("alert", "r");
	fichero_escritura = fopen("local.rules_prueba", "a+");

	if (fichero_lectura != NULL && fichero_escritura != NULL){

		contenido_del_fichero = lee_fichero(fichero_lectura);
		fclose(fichero_lectura);
	
		coincidencias_en_reglas = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
	
		crea_y_escribe_regla(fichero_escritura, coincidencias_en_reglas, "drop");
		fclose(fichero_escritura);	
	}
	else
		fprintf(stderr, "Error al abrir el fichero\n");
	return 0;

} 	

/*-----------------------------------------------
	Función que se encarga de crear las reglas
	a partir de la estructura y la acción a tomar
	buscar si existen coincidencias para esas reglas
	y las escribe

	Recibe: Un fichero, una estructura del tipo
		COINCIDENCIA_REGLA y la acción
	Devuelve: Nada

	Caracteristicas:

-----------------------------------------------*/

void crea_y_escribe_regla(FILE *fichero_escritura, struct COINCIDENCIA_REGLA coincidencias_en_reglas, char *accion){

	
	//Variable para el fichero donde almacenaremos el sid por el que vamos
	FILE *fichero_sid;
	//Variable auxiliar para la lectura del fichero donde almacenaremos el sid
	char c;
	//Array donde almacenaremos el número de sid leido
	char sid[NUM_SID];
	//Variable auxiliar para indicar la posición de la tabla donde nos encontramos
	int pos_sid;
	//Estructura que almacenará los datos relativos al fichero de las reglas
	struct CONTENIDO_FICHERO contenido_del_fichero_reglas;
	//Variable auxiliar para recorrer la tabla devuelta al leer el fichero
	int cont_aux_linea;
	//Array auxiliar para ir formando la regla
		//CAMPOS_REGLAS número de palabras que contendrá como máximo una regla
	char *reglas_aux= (char *) malloc (sizeof (char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para recorre el bucle
	int cont_aux_regla;
	//Bandera para detectar la coincidencia en una regla
	bool coincide;

	//Inicializamos
	cont_aux_regla = INICIO;
	cont_aux_linea = INICIO;
	coincide = true;

	//Leemos el fichero de reglas ya creadas
	contenido_del_fichero_reglas = lee_fichero(fichero_escritura);


	for (cont_aux_regla=0; cont_aux_regla<coincidencias_en_reglas.numero_coincidencias; cont_aux_regla++) {
		
		fichero_sid = fopen("sid", "r");


		if (fichero_sid != NULL){

			
			coincide = true;
			for (cont_aux_linea=0; 
					cont_aux_linea<contenido_del_fichero_reglas.num_frases_fichero; 
					cont_aux_linea++){

				printf("%s\n",contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][0]);
				if (strcmp(accion, contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][0]) == IGUAL){
					printf("Entra\n");	
					coincide = true;
					if (strcmp(coincidencias_en_reglas.protocolo[cont_aux_regla], 
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][1]) != IGUAL ){
						coincide = true;
						if (strcmp (coincidencias_en_reglas.dir_IP[cont_aux_regla],
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][2]) == IGUAL || 
							strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][2]) == IGUAL){
							coincide = true;
							if (strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][3]) == IGUAL){
								coincide = true;
								if (strcmp (coincidencias_en_reglas.dir_IP[cont_aux_regla],
										contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][5]) == IGUAL || 
										strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][5] )== IGUAL) {
									coincide = true;
									if (strcmp("any",  contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][6]) == IGUAL){

										coincide = true;
									}
									else {
										coincide = false;
									}
								}
								else {
									coincide = false;
								}
							}
							else {
								coincide = false;
							}
						}
						else {
							coincide = false;
						}
					}
					else {
						coincide = false;
					}
				}
				else {
					coincide = false;
				}
			}

			if (coincide == false){
				
				strcpy(reglas_aux,accion);
				strcat(reglas_aux, " ");
				strcat(reglas_aux, coincidencias_en_reglas.protocolo[cont_aux_regla]);
				strcat(reglas_aux, " ");
					
				if (coincidencias_en_reglas.dir_en_origen[cont_aux_regla] == true)
					strcat(reglas_aux, coincidencias_en_reglas.dir_IP[cont_aux_regla]);
				else
					strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
					
				//ESTO DEL PUERTO HAY QUE CAMBIARLO CUANDO LO INTRODUZCA EN LA ESTRUCTURA
				strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
				strcat(reglas_aux, "->");
				strcat(reglas_aux, " ");
				//HASTA AQUI

				if (coincidencias_en_reglas.dir_en_origen[cont_aux_regla] == false)
					strcat(reglas_aux, coincidencias_en_reglas.dir_IP[cont_aux_regla]);
				else
					strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
					
				//ESTO DEL PUERTO HAY QUE CAMBIARLO CUANDO LO INTRODUZCA EN LA ESTRUCTURA
				strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
				//HASTA AQUI
				
				strcat(reglas_aux, "(msg: ");
				strcat(reglas_aux,accion);
				strcat(reglas_aux, " ");
				strcat(reglas_aux, coincidencias_en_reglas.protocolo[cont_aux_regla]);
				strcat(reglas_aux, "; sid:");

				pos_sid = INICIO;
				if (fichero_sid !=NULL){
					while ((c=fgetc(fichero_sid))!=EOF){
						if (c != SALTO_DE_LINEA){
							sid[pos_sid] = c;
							pos_sid++;
						}
					}
				}
					
				sprintf(sid, "%d", atoi(sid)+1);
				
				fclose(fichero_sid);
				//system("rm -f sid");
					

				fichero_sid = fopen("sid", "a+");
				//fputs(sid, fichero_sid);
				fclose(fichero_sid);

				strcat(reglas_aux, sid);
				strcat(reglas_aux, "; rev:1;)");
				strcat(reglas_aux, "\n");
				printf("%s\n", reglas_aux);
				//fputs(reglas_aux, fichero_escritura);	
			}		
		}

		else
			fprintf(stderr, "Error al abrir el fichero sid\n");	
	}
	
}


