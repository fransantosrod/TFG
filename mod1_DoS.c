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



int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero_lectura;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct CONTENIDO_ALERTA contenido_fichero_alerta;

	//Inicializamos las variables
	fichero_lectura = fopen("alert_prueba", "r");
	

	if (fichero_lectura != NULL){

		contenido_del_fichero = lee_fichero(fichero_lectura);
		fclose(fichero_lectura);
	
		contenido_fichero_alerta = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");

		crea_y_escribe_regla("local.rules_prueba", contenido_fichero_alerta, "drop");
			
	}

	else
		fprintf(stderr, "Error al abrir el fichero\n");
	return 0;

} 

/*--------------------------------------------------------------
	Función que se encarga de crear las reglas a partir de la 
	estructura y la acción a tomar,
	
	Recibe: El nombre del fichero donde estan  almacenadas las 
		reglas , una estructura del tipo CONTENIDO_ALERTA y 
		la acción
	
	Devuelve: Nada

	Caracteristicas:
		Lo primero que realiza la función es comprobar que 
		la regla que queremos crear no está ya introducida 
		en el fichero de esta forma evitamos tener reglas 
		duplicadas. Tras esto, la crea y la escribe en el 
		fichero. Para crear las reglas es necesario un 
		identificador único (sid) que para guardar siempre 
		una referencia de este se almacena en otro fichero 
		distinto
-----------------------------------------------------------*/

void crea_y_escribe_regla(char *nombre_fichero_escritura, struct CONTENIDO_ALERTA contenido_fichero_alerta, char *accion){


	//Variable para el fichero donde almacenaremos el sid por el que vamos
	FILE *fichero_sid;
	//Variable auxiliar para la lectura del fichero donde almacenaremos el sid
	char c;
	//Array donde almacenaremos el número de sid leido
	char sid[NUM_DIG_SID];
	//Variable auxiliar para indicar la posición de la tabla donde nos encontramos
	int pos_sid;

	//Variable para almacenar el fichero de las reglas, de donde leeremos y escribiremos
	FILE *fichero_escritura;	
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
	coincide = false;

	//Recorremos la estructura de coincidencias que le pasasmos como parámetro
	for (cont_aux_regla=0; cont_aux_regla<contenido_fichero_alerta.numero_lineas; cont_aux_regla++) {
		
		//Abrimos el fichero de las reglas de snort en modo lectura
		fichero_escritura = fopen(nombre_fichero_escritura, "r");
		
		if (fichero_escritura != NULL){
			//Leemos el fichero de reglas ya creadas y almacenamos la info en la estructura
			contenido_del_fichero_reglas = lee_fichero(fichero_escritura);
			fclose(fichero_escritura);
		}
		
	
		//Recorremos la estructura leida del fichero de reglas para buscar coincidencias con la que vamos a introducir
		//Estaremos aquí mientras que no se haya encontrado ninguna coincidencia
		for (cont_aux_linea=0; 
				cont_aux_linea<contenido_del_fichero_reglas.num_frases_fichero && coincide == false; 
				cont_aux_linea++){

			
			//Comprobamos si los valores a introducir coinciden con los que ya están
			/*-----------------------------------------------------------------
				Aprovechando la estructura de las reglas que es la siguiente:
					accion protocolo dir_IP_orig puerto_orig ->
						dir_IP_dest puerto_dest
				Estos valores serán los que comprobaremos
			-----------------------------------------------------------------*/
			if ((strcmp(accion, contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_ACCION]) == IGUAL) &&

			 	(strcmp(contenido_fichero_alerta.protocolo[cont_aux_regla], 
					contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PROTOCOLO]) == IGUAL )&&

				(strcmp (contenido_fichero_alerta.dir_IP[cont_aux_regla],
					contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL || 
					strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL) &&

				
				(strcmp (contenido_fichero_alerta.puerto[cont_aux_regla],
					contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL || 
					strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL) &&

				(strcmp (contenido_fichero_alerta.dir_IP[cont_aux_regla],
					contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST]) == IGUAL || 
					strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST] )== IGUAL) &&

				
				(strcmp (contenido_fichero_alerta.puerto[cont_aux_regla],
					contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL ||
					strcmp("any",  contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL)) {

					//Si esto ocurre, estamos ante una coincidencia en la regla
					coincide = true;
			}

			else {
					//En caso contrario estamos ante una nueva regla
					coincide = false;
			}
		}

		//En el caso en el que no encontramos ninguna coincidencia en el fichero
		if (coincide == false){
				
			//Comenzamos añadiendo la acción que queremos ejecutar en esa regla
			strcpy(reglas_aux,accion);
			strcat(reglas_aux, " ");
			//Añadimos el protocolo 
			strcat(reglas_aux, contenido_fichero_alerta.protocolo[cont_aux_regla]);
			strcat(reglas_aux, " ");
			
			//Comprobamos si cuando detectamos la alerta la dirección estaba en origen o en destino	
			if (contenido_fichero_alerta.dir_en_origen[cont_aux_regla] == true) {
				
				strcat(reglas_aux, contenido_fichero_alerta.dir_IP[cont_aux_regla]);
				strcat(reglas_aux, " ");

				if (contenido_fichero_alerta.dir_Con_Puerto[cont_aux_regla] == true){
					strcat(reglas_aux, contenido_fichero_alerta.puerto[cont_aux_regla]);	
				}
			}
			else{

				strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
				strcat(reglas_aux, "any");
				
			}

			strcat(reglas_aux, " ");
			strcat(reglas_aux, "->");
			strcat(reglas_aux, " ");

			//Realizamos la misma comprobación que anteriormente
			if (contenido_fichero_alerta.dir_en_origen[cont_aux_regla] == false) {

				strcat(reglas_aux, contenido_fichero_alerta.dir_IP[cont_aux_regla]);
				strcat(reglas_aux, " ");
				
				if (contenido_fichero_alerta.dir_Con_Puerto[cont_aux_regla] == true){

					strcat(reglas_aux, contenido_fichero_alerta.puerto[cont_aux_regla]);	
				}
			}

			else{

				strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");
				strcat(reglas_aux, "any");
			}
					
			strcat(reglas_aux, " ");
			
			//Añadimos los campos opcionales como mensaje de información
			/*----------------------------------------------
				El mensaje de info tendrá la estructura de:
					accion + protocolo
			-----------------------------------------------*/
			strcat(reglas_aux, "(msg: ");
			strcat(reglas_aux,accion);
			strcat(reglas_aux, " ");
			strcat(reglas_aux, contenido_fichero_alerta.protocolo[cont_aux_regla]);
			strcat(reglas_aux, "; sid:");

			//Abrimos el fichero donde almacenamos el sid
			/*-----------------------------------------------
				El sid lo debemos almacenar en un fichero
				ya que debe ser único dentro de snort.local
				de esta forma siempre tenemos la referencia
				del último que hemos usado para la última
				regla introducida
			-----------------------------------------------*/
	
			fichero_sid = fopen("sid", "r");

			if (fichero_sid != NULL){
	
				pos_sid = INICIO;
			
				if (fichero_sid !=NULL){
					//Leemos el fichero 
					while ((c=fgetc(fichero_sid))!=EOF){
						if (c != SALTO_DE_LINEA){
							sid[pos_sid] = c;
							pos_sid++;
						}
					}
				}
				//Aumentamos en uno el número que estaba almacenado en el fichero	
				sprintf(sid, "%d", atoi(sid)+1);
				
				fclose(fichero_sid);
	
			}

			else
				fprintf(stderr, "Error al abrir el fichero sid\n");	
	/*	
			//Eliminamos dicho fichero
			system("rm -f sid");
					
			//Lo volvemos a crear y almacenamos el nuevo número
			fichero_sid = fopen("sid", "a+");
			
			if (fichero_sid != NULL){
				fputs(sid, fichero_sid);
				fclose(fichero_sid);
			}
	*/		
			//Almacenamos este sid en la regla que estamos creando
			strcat(reglas_aux, sid);
			//Añadimos el número de revisión, en nuestro caso será siempre 1
			strcat(reglas_aux, "; rev:1;)");
			strcat(reglas_aux, "\n");
			
	/*		//Abrimos el fichero de las reglas y nos situamos al final de este	
			fichero_escritura = fopen(nombre_fichero_escritura, "a+");
			
			//Escribimos la regla que acabamos de crear
			if (fichero_escritura != NULL){
				fputc('\n', fichero_escritura);
				fputs(reglas_aux, fichero_escritura);
				fclose(fichero_escritura);
			}
	*/				
			printf("%s\n", reglas_aux);
		}		

		
	}
	
}	