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



/*---------------------------------------------------------
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	Devuelve: Estructura del tipo COINCIDENCIA_REGLA

	Caracteristicas:
		A través de esta función podremos detectar si 
		en el fichero hemos leido una alerta de una 
		regla cuyo mensaje de información coincida 
		con el string que hemos pasado 
		La estructura que devuelve contiene
			-- numero de conincidencias
			-- Array con todas las direcciones IP
			-- Array con los protocolos
			-- Array con info relativa a la IP
				** true: Estaba en el campo origen
				** false: Estaba en el campo destino
---------------------------------------------------------*/

struct COINCIDENCIA_REGLA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia){
	
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