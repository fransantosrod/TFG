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

#define IGUAL 0
#define FIN_REGLA_ALERTA 3
#define SIGUIENTE_LINEA 1
#define POSICION_IP_ORIGEN 3
#define POSICION_IP_DESTINO 5
#define TAM_IP 4
#define PUNTO '.'
#define TERMINADOR '\0'

//El número máximo de digitos que puede tener un fragmento de IP
#define MAX_DIG_FRAGMENTO_IP 3

char **divide_IP(char *direccion_IP);


int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variable para almacenar la línea en la que se ha detectado la coincidencia de DoS
	int linea_coincidencia;
	//Bandera para detectar cuando se ha producido una coincidencia
	bool flag;


	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;

	//Inicializamos las variables
	fichero = fopen("alert", "r");
	linea_coincidencia = INICIO;
	flag = false;
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;


	contenido_del_fichero = lee_fichero(fichero);
	fclose(fichero);
	
	//Mediante los siguientes bucles recorremos la tabla que nos ha devuelto la estructura
	for (cont_aux_frases_fichero=0; 
		cont_aux_frases_fichero< contenido_del_fichero.num_frases_fichero; 
		cont_aux_frases_fichero++){

		//Si hemos encontrado alguna coincidencia
		if (flag == true) {
			//Si ya no estamos dentro de la info relativa la regla que hemos encontrado
			if (cont_aux_frases_fichero >= linea_coincidencia+FIN_REGLA_ALERTA){
				//Cambiamos el valor de la bandera para poder detectar otra coincidencia
				flag=false;	
				//fprintf(stdout,"	En la línea %d se ha detectado el fin de la coincidencia\n", 
				//	cont_aux_frases_fichero);
			}
		}

		for (cont_aux_palabras_fichero=0; 
			cont_aux_palabras_fichero<= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero]; 
			cont_aux_palabras_fichero++){

			//Comprobamos si hemos leido alguna regla de DoS
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], "DoS") == IGUAL){
				/*-----------------------------------------------------------------------------------------
					En ese caso, por la estructura del fichero "alert" que genera automáticamente SNORT, 
					la primera línea será el mensaje que queremos que aparezca y las siguientes 4 líneas 
					contendrán la información de la regla que ha generado una nueva entrada en el
					documento.
				--------------------------------------------------------------------------------------------*/
				//Indicamos que hemos detectado una coincidencia
				flag = true;
				//Almacenamos la línea en la que la hemos detectado
				linea_coincidencia = cont_aux_frases_fichero;
				//fprintf(stdout,"En la línea %d se ha detectado un ataque DoS\n", 
				//	linea_coincidencia);
			}

			
			/*-----------------------------------------------------------------
				Por estructura del código, también sabemos que la siguiente 
				línea al mensaje de información es la que contiene las 
				direcciones IPs de la comunicación

				Por la misma estructura, conocemos que la IP origen estará en
				la posición 3 de la tabla y la destino en la 5
			-----------------------------------------------------------------*/

			//Comprobamos si hemos detectado una coincidencia y si la línea en la que estamos es la siguiente al mensaje de info
			if (cont_aux_frases_fichero == linea_coincidencia + SIGUIENTE_LINEA && flag==true) {
				if (cont_aux_palabras_fichero == POSICION_IP_ORIGEN || cont_aux_palabras_fichero == POSICION_IP_DESTINO){
					//fprintf(stdout, "%s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					char **dir_dividida = divide_IP(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					for (int i = 0; i<TAM_IP; i++)
						printf("%s\n", dir_dividida[i]);
				}
			}
		}
	}
	return 0;
} 	

char ** divide_IP(char *direccion_IP){
	
	printf("%s\n", direccion_IP);

	//Varible auxiliar para recorrer caracter a caracter la direccion IP
	int caracter_dir_IP;
	//Variable auxiliar para almacenar los dígitos en la posición correcta de la tabla
	int pos_digito;
	//Variable auxiliar para almacenar la posicion donde se debe guardar el digito correspondiente
	//a un fragmento de una direccion IP
	int pos_aux;
	//Array para almacenar la dirección IP dividida
		//4 que son los octetos que contiene una dirección IP
	char *dir_IP_div[TAM_IP];
	//Array auxiliar para almacenar el fragmento de la direccion IP
	char *fragmento_dir_IP = (char *)malloc(sizeof (char)* MAX_DIG_FRAGMENTO_IP);

	caracter_dir_IP = INICIO;
	pos_digito = INICIO;
	pos_aux = INICIO;

	for(caracter_dir_IP = 0; caracter_dir_IP <= (int) strlen(direccion_IP); caracter_dir_IP++){
		
		
		
		if (direccion_IP[caracter_dir_IP] == PUNTO || direccion_IP[caracter_dir_IP] == TERMINADOR){
			
			fragmento_dir_IP[pos_aux++] = TERMINADOR;
			dir_IP_div[pos_digito] = strdup(fragmento_dir_IP);
			
			pos_digito++;
			pos_aux=INICIO;
		}

		else {
			fragmento_dir_IP[pos_aux] = direccion_IP[caracter_dir_IP];
			pos_aux++;
		}
		
		
	} 
	for (int i = 0; i<TAM_IP; i++)
		printf("%s\n", dir_IP_div[i]);
			
	return dir_IP_div;
}