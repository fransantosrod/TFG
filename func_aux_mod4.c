/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares 
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_aux_mod4.h"


/*-------------------------------------------------------
	Función que se encarga de introducir los valores
	deseados en la base de datos

	Recibe: Estructura del tipo CONTENIDO_FICHERO y
		un entero indicando la posición en la 
		que se debe comenzar a recorrer la estructura

	Devuelve: Nada

	Caracterísitcas:
		Mediante un bucle que comienza en la posición
		que se le indica en el segundo argumento,
		crea el comando necesario para introducir
		en la base de datos los valores leidos de 
		los ficheros .dat generados por la herramienta
		ARPWATCH
-------------------------------------------------------*/
void inserta_en_BBDD(struct CONTENIDO_FICHERO contenido_del_fichero, int frase_inicio) {
	
	//String para almacenar el comando
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_REGLAS);
	//Variables auxiliares para recorrer la estrutura leida
	int cont_aux_frases;
	int cont_aux_palabras;


	//Inicializamos
	cont_aux_frases = INICIO;
	cont_aux_palabras = INICIO;


	//Recorremos las nuevas líneas para crear el ficheo que vamos a cargar en la base de datos
	for (cont_aux_frases=frase_inicio; 
			cont_aux_frases<contenido_del_fichero.num_frases_fichero;
			cont_aux_frases++) {
		
		//Construimos el comando para introducir los datos

		//Indicamos la contraseña de la base de datos
		strcpy(comando, "export PGPASSWORD='");
		strcat(comando, CONTRASENIA_BBDD);
		
		//Indicamos el usuario con el que nos queremos conectar
		strcat(comando, "'; psql -U ");
		strcat(comando, USUARIO_BBDD);
		
		//Nombre de la base de datos a la que nos queremos conectar
		strcat(comando, " -d ");
		strcat(comando, NOMBRE_BBDD);

		//Insertamos los dato en la tabla
		strcat(comando, " -c \"INSERT INTO ");
		//Nombre de la tabla dentro de esa base de datos en la que queremos insertar los datos
		strcat(comando, NOMBRE_TABLA_BBDD);
		strcat(comando, " VALUES (");

		//Recorremos los valores obtenidos de los ficheros .dat
		for (cont_aux_palabras=0;
				cont_aux_palabras<=contenido_del_fichero.palabras_por_frase[cont_aux_frases];
				cont_aux_palabras++) {

				//Insertamos los valores que queremos almacenar en la tabla
				if (cont_aux_palabras == FECHA){
					
					//Si estamos en la fecha debemos realizar previamente una conversión
					strcat(comando, "to_timestamp(");
					strcat(comando, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
					strcat(comando, ")");
				}
				
				else {

					//Para la dirección MAC e IP los valores deben ir entrecomillados
					strcat(comando, "'");
					strcat(comando, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
					strcat(comando, "', ");	
				}

			}
		//Terminamos el comando
		strcat(comando, ");\"");
		
		//Redireccionamos la salida
		strcat(comando, " 2>> /tmp/salida_bbdd.txt");
		//Ejecutamos el comando
		system(comando);
	}
	
	//Liberamos memoria
	free(comando);
}