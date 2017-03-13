/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 7-04-2017
Descripción: Fichero que contiene la 
implementación del cuarto módulo que se 
encarga de la gestión de la base de datos
de los clientes asociados 
-------------------------------------------*/

#include "func_aux_mod4.h"


void *mod4 () {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO lista_de_ficheros;
	//Variable auxiliar para almacenar el nombre del fichero que se crea
	char *nuevo_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para almacenar el nombre del fichero sin extensión
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_REGLAS);
	//String para almacenar los comandos auxiliares
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para almacenar el número de líneas leídas en la iteración anterior
	int num_lineas_anteriores;
	//Variables auxiliares para recorrer la estrutura leida
	int cont_aux_frases;
	


	//Inicializamos
	num_lineas_anteriores = INICIO;
	cont_aux_frases = INICIO;
	strcpy(nombre_fichero, FICHERO_REGISTRO_CLIENTES);

	while(true){
		/*-------------------------------------------------------
			Obtenemos todos los registros almacenados en ARPWATCH
			Nos interesa únicamente las tres primeras columnas
				-- En la primera tenemos la dirección MAC
				-- En la segunda la dirección IP
				-- En la tercera el instante de tiempo en el que 
					se almacenó
		-------------------------------------------------------*/
		strcpy(comando, "cat /var/lib/arpwatch/*.dat | cut -f 1,2,3 > ");
		strcat(comando, FICHERO_REGISTRO_CLIENTES);
		system(comando);

		strcpy(comando, "cat /var/lib/arpwatch/*.dat- | cut -f 1,2,3 >> ");
		strcat(comando, FICHERO_REGISTRO_CLIENTES);
		system(comando);
		
		//Leemos el nombre del fichero en el que hemos almacenado todos lo datos
		contenido_del_fichero = lee_fichero(nombre_fichero);

		//Comprobamos que el fichero no está en el límite
		if (contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){


			//Comprobamos que ha aumentado el número de clientes registrado desde la última vez que lo leimos
			if (contenido_del_fichero.num_frases_fichero > num_lineas_anteriores) {
				
				//Cargamos los nuevos datos en la base de datos
					
				inserta_en_BBDD(contenido_del_fichero, num_lineas_anteriores);
					
			}
		}

		//Si estamos cerca del límite
		else {
			
			//Vaciamos el fichero de registro de clientes
			nuevo_fichero = vacia_fichero(nombre_fichero);
			
			//Leemos el nombre del fichero en el que hemos almacenado todos lo datos
			contenido_del_fichero = lee_fichero(nuevo_fichero);
			
			//Comprobamos que ha aumentado el número de clientes registrado desde la última vez que lo leimos
			if (contenido_del_fichero.num_frases_fichero > num_lineas_anteriores) {
				
				//Cargamos los nuevos datos en la base de datos
					
				inserta_en_BBDD(contenido_del_fichero, num_lineas_anteriores);	
					
			}

			
			/*-------------------------------------------------------
				También es necesario vaciar los ficheros .dat ya
				que si no se vacian en la próxima iteración se
				volverá a llenar el fichero de registro ya que 
				se vuelca el contenido de todos los ficheros
			-------------------------------------------------------*/	
			//Obtenemos todos los ficheros .dat existentes
			system("ls /var/lib/arpwatch/*.dat > lista_ficheros");
			system("ls /var/lib/arpwatch/*.dat- >> lista_ficheros");

			//Leemos el fichero donde hemos almacenado todos los ficheros
			lista_de_ficheros = lee_fichero("lista_ficheros");

			//Recorremos todos los dicheros existentes
			for (cont_aux_frases=0; 
				cont_aux_frases<lista_de_ficheros.num_frases_fichero;
				cont_aux_frases++) {

				//Vaciamos el fichero y lo almacenamos en otro auxiliar
				vacia_fichero(lista_de_ficheros.contenido_leido_del_fichero[cont_aux_frases][INICIO]);
			}

			//Cambiamos el valor de líneas leídas a 0
			contenido_del_fichero.num_frases_fichero = INICIO;
		
		}

		//Almacenamos el número de líneas leidas
		num_lineas_anteriores = contenido_del_fichero.num_frases_fichero;
		
		sleep(INTERVALO_LECTURA);
	}

	//Liberamos memoria
	free(nuevo_fichero);
	free(nombre_fichero);
	free(comando);

	pthread_exit(NULL);
}
