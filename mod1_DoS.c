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


#define FIN_REGLA_ALERTA 3
#define SIGUIENTE_LINEA 1
#define POSICION_IP_ORIGEN 3
#define POSICION_IP_DESTINO 5

void comprueba_Coincidencia_Alertas(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia);

int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	
	//Inicializamos las variables
	fichero = fopen("alert", "r");
	

	contenido_del_fichero = lee_fichero(fichero);
	fclose(fichero);

	comprueba_Coincidencia_Alertas(contenido_del_fichero, "DoS");


	return 0;

} 	


/*---------------------------------------------------------
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	Devuelve: Nada

	Caracteristicas:
		A través de esta función podremos detectar si 
		en el fichero hemos leido una alerta de una 
		regla cuyo mensaje de información coincida 
		con el string que hemos pasado 
---------------------------------------------------------*/

void comprueba_Coincidencia_Alertas(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia){
	
	//Variable para almacenar la línea en la que se ha detectado la coincidencia de DoS
	int linea_coincidencia;
	//Bandera para detectar cuando se ha producido una coincidencia
	bool flag;
	//Bandera para detectar si la red es local
	bool red_local;

	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;

	//Inicializamos las variables
	linea_coincidencia = INICIO;
	flag = false;
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;
	red_local=false;


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
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], nombre_Coincidencia) == IGUAL){
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
					red_local=comprueba_IP(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					if (red_local == true)
						fprintf(stdout, "%s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
				}
			}
		}
	}	
}