/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 21-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usarán en el segundo módulo
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "func_aux.h"
#include "func_aux_mod2.h"

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

struct ESTRUCTURA_REGLA busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero) {

	//Estructura que se va a devolver para almacenar los datos de las reglas que queremos crear
	struct ESTRUCTURA_REGLA informacion_regla;
	//Variable auxiliar para recorrer las frases leidas
	int cont_aux_frases;
	//Variable auxiliar para recorrer las palabras de cada frase
	int cont_aux_palabras;
	//Variable para almacenar la linea en la que hemos detectado la coincidencia
	int linea_coincidencia;
	//Variable para almacenar el número dentro del array en el que detectamos la coincidencia
	int palabra_coincidencia;
	//Contador auxiliar para almacenar el número de reglas de las que hemos almacenado información
	int num_reglas;
	//Variable auxiliar para almacenar la dirección MAC antigua
	//char mac_antigua[TAM_MAC];
	//Bandera para indicar si hemos detectado una coincidencia de cambio
	bool flag_cambio_MAC;
	//Bandera para indicar si hemos detectado una coincidencia de cambio inmediato
	bool flag_cambio_MAC_Inmediato;


	//Inicializamos
	cont_aux_frases = INICIO;
	cont_aux_palabras = INICIO;
	linea_coincidencia = INICIO;
	palabra_coincidencia= INICIO;
	num_reglas=INICIO;
	flag_cambio_MAC = false;
	flag_cambio_MAC_Inmediato = false;

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
		if (cont_aux_frases == linea_coincidencia+SIGUIENTE_LINEA && (flag_cambio_MAC ==true || flag_cambio_MAC_Inmediato == true)){
			//Cambiamos el valor de la bandera para señalizar el instante en el que encontramos otra
			flag_cambio_MAC = false;
			flag_cambio_MAC_Inmediato = false;

		}

		//Recorremos las paralbras que hemos leido dentro de cada frase
		for(cont_aux_palabras=0; 
			cont_aux_palabras <= contenido_del_fichero.palabras_por_frase[cont_aux_frases];
			cont_aux_palabras++) {
/*-------------------------------------------
	Si en un futuro queremos almacenar
	la dirección MAC e IP de cada cambio
	bastará con descomentar este apartado
-------------------------------------------*/

/*
	
			//Comprobamos si la palabra actual es la que estamos buscando y que nos indica el cambio de MAC para una misma dir_IP 
			if (flag_cambio_MAC == false && strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras],
				CAMBIO_EN_MAC) == IGUAL){

				//Almacenamos la línea en la que hemos encontrado la coincidencia
				linea_coincidencia = cont_aux_frases;
				//Almacenamos la palabra en la que hemos encontrado la coincidencia
				palabra_coincidencia = cont_aux_palabras;
				//Cambiamos el valor de la bandera para indicar que hemos encontrado una coincidencia
				flag_cambio_MAC = true;
			}

			----------------------------------------------------------------
				Detectamos si ARPWATCH nos ha notificado de un cambio
				en la dirección MAC para una IP que tenía almacenada 
				anteriormente
			----------------------------------------------------------------
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección IP
			if ( flag_cambio_MAC == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_IP){
				printf("Dirección IP %s con el problema: %s\n", 
					contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras], CAMBIO_EN_MAC);
			}

			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC nueva
			else if (flag_cambio_MAC == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_NUEVA){
				printf("	Dirección MAC nueva: %s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
			}
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC antigua
			else if (flag_cambio_MAC == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_VIEJA){
				
				//Eliminamos los paréntesis con los que ARPWATCH nos notifica en los logs sobre la antigua MAC
				int i=0;
				
				for (i=1; i < (int)strlen(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras])-1;i++){
					
					mac_antigua[i-1] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras][i];
					
				}
				mac_antigua[i-1] = TERMINADOR;
				printf("	Dirección MAC vieja: %s\n", mac_antigua);
			}

*/
			/*--------------------------------------------------------------
				Detectamos si ARPWATCH nos ha notificado de un cambio 
				en la dirección MAC en un intervalo corto de tiempo
				para una misma dirección IP
			--------------------------------------------------------------*/
			//Comprobamos si hemos detectado algún cambio inmediato de MAC
			if (flag_cambio_MAC_Inmediato == false && strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras],
				CAMBIO_MAC_INMEDIATO ) == IGUAL){

				//Almacenamos la línea en la que hemos encontrado la coincidencia
				linea_coincidencia = cont_aux_frases;
				//Almacenamos la palabra en la que hemos encontrado la coincidencia
				palabra_coincidencia = cont_aux_palabras;
				//Cambiamos el valor de la bandera para indicar que hemos encontrado una coincidencia
				flag_cambio_MAC_Inmediato = true;
			}

			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección IP
			if ( flag_cambio_MAC_Inmediato == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_IP_INM){
				
				//Almacenamos la dirección IP
				informacion_regla.dir_IP[num_reglas] = 
					strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
				
				//Indicamos que la dirección IP no lleva asociado ningún puerto
				informacion_regla.dir_Con_Puerto[num_reglas] = false;

				//Indicamos que queremos que la dirección IP vaya en destino
				informacion_regla.dir_en_origen[num_reglas] = false;

				//Indicamos que el protocolo sea IP para que cubra a todos
				informacion_regla.protocolo[num_reglas] = strdup(IP);

				//Aumentamos el número de reglas
				num_reglas++;
				
			}

/*-------------------------------------------
	Si en un futuro queremos almacenar
	la dirección MAC bastará con
	descomentar este apartado
-------------------------------------------*/

/*
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC nueva
			else if (flag_cambio_MAC_Inmediato == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_NUEVA_INM){
				printf("	Dirección MAC nueva: %s\n", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
			}
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección MAC antigua
			else if (flag_cambio_MAC_Inmediato == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_MAC_VIEJA_INM){
				
				//Eliminamos los paréntesis con los que ARPWATCH nos notifica en los logs sobre la antigua MAC
				int i=0;
				
				for (i=1; i < (int)strlen(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras])-1;i++){
					
					mac_antigua[i-1] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras][i];
					
				}
				mac_antigua[i-1] = TERMINADOR;
				printf("	Dirección MAC vieja: %s\n", mac_antigua);
			}

*/
			
		}
	}

	//Almacenamos el número de reglas que deseamos crear
	informacion_regla.numero_lineas = num_reglas;
	return informacion_regla;
}