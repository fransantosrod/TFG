/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares 
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "func_aux.h"

/* -------------------------------------------------------
	Función que se encarga de la lectura de los ficheros 
	Recibe una fichero abierto para su lectura
	Devuelve: Una estructura con
		-- Número de frases leidas
		-- Número de palabras en cada frase
		-- Array doble que almacena cada palabra de cada frase

	Características:
		El fichero no debe contener más de NUM_FRASES
		formada por no más de NUM_PALABRAS_POR_FRASE
		cada palabra de las frase no debe tener más de
		NUM_CARACTERES_PALABRA, estos parámetros están definidos
		en la estructura
--------------------------------------------------------*/

struct CONTENIDO_FICHERO lee_fichero (FILE *fichero_lectura){

	//Variable para leer el fichero caracter a carater
	char c;
	//Variable para almacenar el valor del caracter anterior
	char c_ant;
	
	//Variable para contabilizar el número de frases del fichero
	int frase;
	//Variable para contabilizar en que palabra estamos
	int palabra;
	//Variable para contabilizar el número de letras
	int letra;
	//Bandera para bloquear el aumento de frases si estamos leyendo un salto de parrafo
	bool flag;
	
	//Array auxiliar para almacenar cada palabra de la frase
		//NUM_CARACTERES_PALABRA caracteres por palabras
	char *palabras_frase = (char *)malloc (sizeof(char)*NUM_CARACTERES_PALABRA);
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;

	//Inicialización de variables
	frase = INICIO;
	c_ant = ESPACIO;
	palabra = INICIO;
	letra = INICIO;
	flag = false;


	if (fichero_lectura != NULL){
		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF){
		
			if (c!= ESPACIO && c!=SALTO_DE_LINEA){
				//Almacenamos el caracter leido
				palabras_frase[letra] = c;
				//Aumentamos el número de letras leidas
				letra++;
			}
		
			//Comprobamos si el caracter leido anteriormente y el actual son un salto de linea
			else if (c_ant == SALTO_DE_LINEA && c == SALTO_DE_LINEA){

				//Si lo es, estamos en un nuevo parrafo
				flag=true;
				palabra=INICIO;
			
			}
		
			//Comprobamos si se ha leido alguna letra,si esta esta es un salto de línea y no estamos en el salto de parrafo 
			else if (c == SALTO_DE_LINEA && letra>0 && flag!= true){
			
				//Almacenamos la palabra de la frase deseada en nuestra tabla
				palabras_frase[letra++] = '\0';
				contenido_del_fichero.contenido_leido_del_fichero[frase][palabra] = strdup(palabras_frase); 
				
				//Volvemos a iniciar el número de letras ya que estamos en una nueva frase y palabra
				letra = INICIO;
				//Almacenamos el número de palabras que tenía la frase que hemos terminado de leer
				contenido_del_fichero.palabras_por_frase[frase] = palabra;
				
				palabra=INICIO;	
			
				//En ese caso aumentamos el número de frases leidas
				frase++;
			
			}
		
			//Comprobamos si el caracter es un espacio y si se ha leido alguno 
			else if (c == ESPACIO && letra >0){
			
				//Almacenamos la palabra de la frase deseada en nuestra estructura
				palabras_frase[letra++] = '\0';
				contenido_del_fichero.contenido_leido_del_fichero[frase][palabra] = strdup(palabras_frase); 
				
				//Aumentamos el número de palabras leidas
				palabra++;
				//Volvemos a iniciar el número de letras leidas ya que estamos en una nueva palabra
				letra=INICIO;
			}

			c_ant=c;;
			flag=false;
		}
	}

	else 
		fprintf(stderr,"No se puede abrir el fichero para su lectura\n");
	
	//Rellenamos la estructura para poder usarla en otro punto del codigo
	contenido_del_fichero.num_frases_fichero = frase;

	//Liberamos la memoria
	free(palabras_frase);
	
	return contenido_del_fichero;
}




/*-------------------------------------
Función que se encarga de comprobar si una
dirección IP pertenece a la red local o no

Recibe: char* que contiene la dirección IP
que se quiere comprobar
Devuelve: Un booleano indicando si pertenece o no
	-- true: Pertenece a la red local
	-- false: No pertenece a la red local

Caracteristicas: 
	Lo primero que realiza es la fragmentación
	de la dirección IP
	Tras esto comprueba si los tres primeros
	fragmentos coinciden con la red local
		10.10.10.0/24 en este caso
	Devuelve el si es cierto o no
-------------------------------------*/
bool comprueba_IP(char *direccion_IP){


	//Varible auxiliar para recorrer caracter a caracter la direccion IP
	int caracter_dir_IP;
	//Variable auxiliar para almacenar los dígitos en la posición correcta de la tabla
	int pos_digito;
	//Variable auxiliar para almacenar la posicion donde se debe guardar el digito correspondiente
	//a un fragmento de una direccion IP
	int pos_aux;
	//Bandera que nos indica si la red es local o no
	bool red_local;

	//Array para almacenar la dirección IP dividida
		//4 que son los octetos que contiene una dirección IP
	char *dir_IP_div[TAM_IP];
	//Array auxiliar para almacenar el fragmento de la direccion IP
	char *fragmento_dir_IP = (char *)malloc(sizeof (char)* MAX_DIG_FRAGMENTO_IP);

	//Inicializamos las variables
	caracter_dir_IP = INICIO;
	pos_digito = INICIO;
	pos_aux = INICIO;
	red_local=false;

	//Bucle para realizar la fragmentación de la dirección IP
	//Recorremos caracter a caracter el string que le hemos pasado como parámetro
	for(caracter_dir_IP = 0; caracter_dir_IP <= (int) strlen(direccion_IP); caracter_dir_IP++){
			
		//Comprobamos si el caracter que estamos leyendo es . o estramos en el último caracter del string	
		if (direccion_IP[caracter_dir_IP] == PUNTO || direccion_IP[caracter_dir_IP] == TERMINADOR){
			//Almacenamos el fragmento en la tabla
			fragmento_dir_IP[pos_aux++] = TERMINADOR;
			dir_IP_div[pos_digito] = strdup(fragmento_dir_IP);
			
			//Aumentamos la posición del fragmento
			pos_digito++;
			//Iniciamos el caracter auxilar
			pos_aux=INICIO;
		}

		else {

			//Almacenamos el caracter leido en la tabla auxiliar
			fragmento_dir_IP[pos_aux] = direccion_IP[caracter_dir_IP];
			pos_aux++;
		}
	} 
	
	//Una vez que tenemos fragmentada la dirección IP comprobamos si los tres primeros coinciden con la red local
	/*-----------------------------------------------------------------
		En este caso al tener definida la red local como un /24 sólo 
		debemos mirar los tres primeros fragmentos ya que el último 
		es el variable
	-----------------------------------------------------------------*/
	if ((strcmp(dir_IP_div[FRAG_1],VALOR_FRAG_1)==IGUAL)  && (strcmp(dir_IP_div[FRAG_2],VALOR_FRAG_2)==IGUAL) 
		&& (strcmp(dir_IP_div[FRAG_3],VALOR_FRAG_3)==IGUAL))
		red_local=true;
	
	else
		red_local =false;
	
	return red_local;
}