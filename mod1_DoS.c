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
#define LINEA_PROTOCOLO 2
#define POSICION_IP_ORIGEN 3
#define POSICION_IP_DESTINO 5
#define NUM_REGLAS 100
#define UDP "UDP"
#define TCP "TCP"
#define ICMP "ICMP"

typedef struct COINCIDENCIA_REGLA {
	
	//Array para almacenar la dirección IP
	char *dir_IP[NUM_REGLAS];
	//Array de string para almacenar el protocolo
	char *protocolo[NUM_REGLAS];
	/*--------------------------------------
		Bandera para conocer si la dirección
		IP se encontraba en el campo de origen
		o en el de destino
			-- true: se encontraba en origen
			-- false: se encontraba en destino
	--------------------------------------*/
	bool dir_en_origen[NUM_REGLAS];
	//Variable para almacenar el número total de coincidencias
	int numero_coincidencias;

}coincidencia_regla;



struct COINCIDENCIA_REGLA comprueba_Coincidencia_DoS(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia);

int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct COINCIDENCIA_REGLA coincidencias_en_reglas;

	//Inicializamos las variables
	fichero = fopen("alert", "r");
	

	contenido_del_fichero = lee_fichero(fichero);
	fclose(fichero);

	coincidencias_en_reglas = comprueba_Coincidencia_DoS(contenido_del_fichero, "DoS");
	for (int i = 0; i< coincidencias_en_reglas.numero_coincidencias; i++){

		printf("Direccion IP %s encontrada en origen: %d perteneciente al protocolo %s\n", 
			coincidencias_en_reglas.dir_IP[i], coincidencias_en_reglas.dir_en_origen[i], coincidencias_en_reglas.protocolo[i]);
	}
	
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

struct COINCIDENCIA_REGLA comprueba_Coincidencia_DoS(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia){
	
	//Variable para almacenar la línea en la que se ha detectado la coincidencia de DoS
	int linea_coincidencia;
	//Bandera para detectar cuando se ha producido una coincidencia
	bool flag;
	//Bandera para detectar si la red es local
	bool red_local;
	//Estructura que alamcena los datos relativos a las coincidencias
	struct COINCIDENCIA_REGLA coincidencia_en_regla;
	//Variable auxiliar para almacenar el número de coincidencias y recorrer la estructura
	int numero_de_coincidencia;

	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;

	//Inicializamos las variables
	linea_coincidencia = INICIO;
	flag = false;
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;
	red_local=false;
	numero_de_coincidencia=INICIO;

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
			}
		}

		for (cont_aux_palabras_fichero=0; 
			cont_aux_palabras_fichero<= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero]; 
			cont_aux_palabras_fichero++){

			//Comprobamos si hemos leido alguna regla de DoS
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], nombre_Coincidencia) 
				== IGUAL){
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
			}

			/*-----------------------------------------------------------------
				Por estructura del fichero, también sabemos que la siguiente 
				línea al mensaje de información es la que contiene las 
				direcciones IPs de la comunicación

				Por la misma estructura, conocemos que la IP origen estará en
				la posición 3 de la tabla y la destino en la 5
			-----------------------------------------------------------------*/

			//Comprobamos si hemos detectado una coincidencia y si la línea en la que estamos es la siguiente al mensaje de info
			if (cont_aux_frases_fichero == linea_coincidencia + SIGUIENTE_LINEA && flag==true) {
				
				//Una vez que estamos en la línea comprobamos si nos encontramos en el campo correspondiente a una dirección IP
				if (cont_aux_palabras_fichero == POSICION_IP_ORIGEN || cont_aux_palabras_fichero == POSICION_IP_DESTINO){
					
					//Comprobamos si la dirección IP que acabamos de leer es de la red local
					red_local=comprueba_IP(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					
					//En caso que sea de la red externa
					if (red_local == false){
						//Comprobamos si dicha dirección IP estaba en el campo correspondiente a destino u origen
						if (cont_aux_palabras_fichero == POSICION_IP_ORIGEN){
							coincidencia_en_regla.dir_en_origen[numero_de_coincidencia]=true;
						}
						else{
							coincidencia_en_regla.dir_en_origen[numero_de_coincidencia]=false;	
						}
						//Almacenamos dicha dirección IP en la estructura
						coincidencia_en_regla.dir_IP[numero_de_coincidencia] = 
							strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
						
					}
				}
			}


			/*--------------------------------------------------------
				Aprovechando de nuevo la estructura del fichero,
				sabemos que el protocolo lo encontramos en la tercera 
				línea por lo que debemos buscar ahí
			--------------------------------------------------------*/
			
			//Comprobamos que estamos en la línea donde podemos comprobar el protocolo que hemos encontrado una coincidencia
			//	y que la dirección IP anterior era una NO local
			if (cont_aux_frases_fichero == linea_coincidencia + LINEA_PROTOCOLO && flag==true && red_local==false){
				
				//Cambiamos el valor de la bandera para volver a entrar solo en el caso que se detecte una IP no local
				red_local=true;

				//Comprobamos a qué protocolo pertenece la línea que estamos leyendo por si hay coincidencia tenerlo almacenado
				if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], UDP) 
					== IGUAL){
					coincidencia_en_regla.protocolo[numero_de_coincidencia] = strdup(UDP);
				}
				else if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], TCP) 
					== IGUAL){
					coincidencia_en_regla.protocolo[numero_de_coincidencia] = strdup(TCP);
				}
				else if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], ICMP) 
					== IGUAL){

					coincidencia_en_regla.protocolo[numero_de_coincidencia]= strdup(ICMP);
				}
				//Aumentamos el número de coincidencias para el string dado al principio
				numero_de_coincidencia++;
			}			

		}
	}	
	coincidencia_en_regla.numero_coincidencias = numero_de_coincidencia;

	return coincidencia_en_regla;
}