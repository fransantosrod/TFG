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
	Recibe: El nombre del fichero a leer
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

struct CONTENIDO_FICHERO lee_fichero (char *nombre_fichero){

	//Variable donde almacenaremos el fichero que queremos leer
	FILE *fichero_lectura;
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
	//Bandera para evitar que se lean más líneas que el tamaño de la tabla
	bool sobrepasa_tamanio;
	

	//Array auxiliar para almacenar cada palabra de la frase
		//NUM_CARACTERES_PALABRA caracteres por palabras
	char *palabras_frase = (char *)malloc (sizeof(char)*NUM_CARACTERES_PALABRA);
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;

	//Inicialización de variables
	fichero_lectura = fopen(nombre_fichero, "r");
	frase = INICIO;
	c_ant = ESPACIO;
	palabra = INICIO;
	letra = INICIO;
	flag = false;
	sobrepasa_tamanio = false;

	if (fichero_lectura != NULL){
		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF && sobrepasa_tamanio==false){
			
			if (frase < NUM_FRASES){
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

			else {

				sobrepasa_tamanio = true;
				fprintf(stderr, "El fichero %s supera las %d líneas útiles máximas\n", nombre_fichero, NUM_FRASES);
			}
		}
		fclose(fichero_lectura);
	}

	else 
		fprintf(stderr,"No se puede abrir el fichero para su lectura\n");
	
	//Rellenamos la estructura para poder usarla en otro punto del codigo
	contenido_del_fichero.num_frases_fichero = frase;

	//Liberamos la memoria
	free(palabras_frase);
	
	return contenido_del_fichero;
}



/*---------------------------------------------------------
	
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	Devuelve: Estructura del tipo CONTENIDO_ALERTA

	Caracteristicas:
		A través de esta función podremos detectar si 
		en el fichero hemos leido una alerta de una 
		regla cuyo mensaje de información coincida 
		con el string que hemos pasado 
		La estructura que devuelve contiene
			-- Numero de conincidencias
			-- Array con todas las direcciones IP
			-- Array con los protocolos
			-- Array con los puertos
			-- Bandera con info relativa al puerto
				--true: Esa regla tiene un puerto asociada
				--false: no lo tiene
			-- Bandera con info relativa a la IP
				** true: Estaba en el campo origen
				** false: Estaba en el campo destino
---------------------------------------------------------*/

struct CONTENIDO_ALERTA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia){
	
	//Variable para almacenar la línea en la que se ha detectado la coincidencia de DoS
	int linea_coincidencia;
	//Bandera para detectar cuando se ha producido una coincidencia
	bool flag;
	//Bandera para detectar si la red es local
	bool red_local;
	//Bandera para almacenar cuando se detecta una red externa
	bool detectada_red_externa;
	//Estructura que alamcena los datos relativos a las coincidencias
	struct CONTENIDO_ALERTA contenido_fichero_alerta;
	//Variable auxiliar para almacenar el número de coincidencias y recorrer la estructura
	int numero_de_coincidencia;

	//Variable auxiliar para almacenar la dirección del caracter : y el puerto
	char *puerto;
	//Variable auxiliar para almacenar la dirección IP cuando
	char *dir_IP_aux = (char *)malloc(sizeof (char *) * TAM_IP*MAX_DIG_FRAGMENTO_IP);

	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;

	
	//Inicializamos las variables
	linea_coincidencia = INICIO;
	flag = false;
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;
	red_local=false;
	detectada_red_externa = false;
	numero_de_coincidencia=INICIO;

	//Mediante los siguientes bucles recorremos la tabla que nos ha devuelto la estructura
	for (cont_aux_frases_fichero=0; 
		cont_aux_frases_fichero< contenido_del_fichero.num_frases_fichero && numero_de_coincidencia < NUM_REGLAS; 
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
			cont_aux_palabras_fichero<= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero] && numero_de_coincidencia < NUM_REGLAS; 
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
				direcciones IPs  y los puertos de la comunicación

				Por la misma estructura, conocemos que la IP origen estará en
				la posición 3 de la tabla y la destino en la 5

				Los puertos están en la misma posición que las direcciones IP
				pero tras un :	
			-----------------------------------------------------------------*/

			//Comprobamos si hemos detectado una coincidencia y si la línea en la que estamos es la siguiente al mensaje de info
			if (cont_aux_frases_fichero == linea_coincidencia + SIGUIENTE_LINEA && flag==true) {
				
				//Una vez que estamos en la línea comprobamos si nos encontramos en el campo correspondiente a una dirección IP
				if (cont_aux_palabras_fichero == POSICION_IP_ORIGEN || cont_aux_palabras_fichero == POSICION_IP_DESTINO){
					
					//Buscamos en este campo el caracter :
					puerto =
						strchr(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], DOS_PUNTOS);
					
					
					//Si hemos encontrado dicho caracter
					if (puerto != NULL) {
						
						//Copiamos la dirección IP
						snprintf(dir_IP_aux, 
							(int)strlen(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]) - 
							(int)strlen(puerto)+1 , 
							"%s",contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);

						
						//Bucle para elminar el caracter : del primer campo de la cadena
						int i=0;
						for ( i =0; i<(int)strlen(puerto)-1;i++){
							puerto[i]=puerto[i+1];
						}

						//Añadimos el terminador
						puerto[(int)strlen(puerto)-1] = TERMINADOR;
						
					}

					//En el caso en el que no hayamos encontrado el caracter
					else {
						

						//Almacenamos la dirección IP en la variable auxiliar
						dir_IP_aux = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					}


					//Comprobamos si la dirección IP que acabamos de leer es de la red local
					red_local=comprueba_IP(dir_IP_aux);
					
					//En caso que sea de la red externa
					if (red_local == false){

						//Indicamos que hemos detectado una red externa
						detectada_red_externa=true;

						//Almacenamos dicha dirección IP en la estructura
						contenido_fichero_alerta.dir_IP[numero_de_coincidencia] = 
							strdup(dir_IP_aux);
						

						//Comprobamos si dicha dirección IP estaba en el campo correspondiente a destino u origen
						if (cont_aux_palabras_fichero == POSICION_IP_ORIGEN){
							contenido_fichero_alerta.dir_en_origen[numero_de_coincidencia]=true;
						}
						else{
							contenido_fichero_alerta.dir_en_origen[numero_de_coincidencia]=false;	
						}

						if (puerto != TERMINADOR){
							
							//Indicamos a través de la bandera que esta regla si contiene puerto
							contenido_fichero_alerta.dir_Con_Puerto[numero_de_coincidencia] = true;
							
							//Almacenamos el número de puerto en la estructura
							contenido_fichero_alerta.puerto[numero_de_coincidencia] = strdup(puerto);
						
						}
						else{
							
							//Indicamos a través de la bandera que no lo hemos encontrado
							contenido_fichero_alerta.dir_Con_Puerto[numero_de_coincidencia] = false;
							
							//En este caso, en la estructura almacenamos un terminador para que no quede vacio
							contenido_fichero_alerta.puerto[numero_de_coincidencia] = strdup("\0");
						}
						

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
			if (cont_aux_frases_fichero == linea_coincidencia + LINEA_PROTOCOLO && flag==true && detectada_red_externa==true){
				
				//Cambiamos el valor de la bandera para volver a entrar solo en el caso que se detecte una IP no local
				detectada_red_externa=false;

				//Comprobamos a qué protocolo pertenece la línea que estamos leyendo por si hay coincidencia tenerlo almacenado
				if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], UDP) 
					== IGUAL){
					contenido_fichero_alerta.protocolo[numero_de_coincidencia] = strdup(UDP);
				}
				else if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], TCP) 
					== IGUAL){
					contenido_fichero_alerta.protocolo[numero_de_coincidencia] = strdup(TCP);
				}
				else if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero], ICMP) 
					== IGUAL){

					contenido_fichero_alerta.protocolo[numero_de_coincidencia]= strdup(ICMP);
				}
				//Aumentamos el número de coincidencias para el string dado al principio
				numero_de_coincidencia++;
			}			

		}
	}	
	
	contenido_fichero_alerta.numero_lineas = numero_de_coincidencia;

	//Liberamos la memoria
	free(dir_IP_aux);

	return contenido_fichero_alerta;
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

	//Liberamos memoria
	free(fragmento_dir_IP);
	return red_local;
}


/*-----------------------------------------------------
	Función que se encarga de buscar en el fichero 
	de reglas donde se quiera insertar si existe 
	una igual para evitar introducir reglas duplicadas

	Recibe: Estructura del tipo CONTENIDO_FICHERO,
	estructura del tipo CONTENIDO_ALERTA, una acción
	y la posición dentro de la estructura CONTENIDO_ALERTA
	donde se encuentra leyendo la info relativa a la nueva
	regla

	Devuelve: Un booleano
		--true: Ya hay una regla igual
		-- false: No la hay

	Caracteristicas:
		Mediante un bucle, recorre el fichero de las
		reglas y va comprobando una a una si la 
		info que tiene almacenada para crear la nueva
		regla coincide con alguna ya creada para ello,
		va avanzando campo a campo de cada regla hasta
		llegar al puerto destino, la info opcional
		no se evalua ya que esta puede ser muy 
		cambiante
-----------------------------------------------------*/

bool comprueba_Regla(struct CONTENIDO_FICHERO contenido_del_fichero_reglas, struct CONTENIDO_ALERTA contenido_fichero_alerta, 
	char *accion, int pos_dentro_cont_alerta) {

	
	//Bandera para detectar la coincidencia en una regla
	bool coincide;
	//Variable auxiliar para recorrer la tabla devuelta al leer el fichero
	int cont_aux_linea;
	
	//Inicializamos las variables
	cont_aux_linea = INICIO;
	coincide = false;
	

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

		//Comprobamos conjuntamente si la pareja acción protocolo son iguales
		if ((strcmp(accion, contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_ACCION]) == IGUAL) &&
		 	(strcmp(contenido_fichero_alerta.protocolo[pos_dentro_cont_alerta], 
				contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PROTOCOLO]) == IGUAL )){
				
				//Comprobamos si la dirección IP estaba en el campo origen	
				if (contenido_fichero_alerta.dir_en_origen[pos_dentro_cont_alerta] == true){

					//En ese caso, comprobamos que coincida la dir_IP con la origen de la regla y la destino con "any"
					if (strcmp (contenido_fichero_alerta.dir_IP[pos_dentro_cont_alerta],
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL ||
						strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST] )== IGUAL){

						//Comprobamos si la dir_IP tenía asociada un puerto
						if (contenido_fichero_alerta.dir_Con_Puerto[pos_dentro_cont_alerta] == true){

							//Si es así comprobamos si el puerto coincide
							if (strcmp (contenido_fichero_alerta.puerto[pos_dentro_cont_alerta],
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL ){

								//Si todo lo anterior coincide, la regla ya existía
								coincide = true;
							}

							else {
								//En caso contrario, estamos ante una nueva regla
								coincide = false;
							}
						}

						else {
								
							//Si no tenía asociada ningún puerto, comprobamos que es "any"
							if (strcmp("any", 
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL) {
								
								//Si coincide, estamos ante una regla ya existente
								coincide =true;
	
							}
								
							else {

								//En caso de que no coincida, es una nueva regla
								coincide = false;
	
							}
						}
					}

					else {
						
						//En caso contrario, estamos ante una nueva regla
						coincide = false;
	
					}
	
				}
				
				//Si la dirección IP no estaba en el campo origen
				else { 
					
					//Comprobamos que el campo origen sea "any" y el destno la dir_IP de la regla que queremos crear
					if (strcmp("any", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL &&
						strcmp (contenido_fichero_alerta.dir_IP[pos_dentro_cont_alerta],
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST]) == IGUAL ){

						//Comprobamos si esa dirección llevaba asociada un puerto
						if (contenido_fichero_alerta.dir_Con_Puerto[pos_dentro_cont_alerta] == true){

							//Si lo llevaba asociado, comprobamos si el puerto coincide
							if (strcmp (contenido_fichero_alerta.puerto[pos_dentro_cont_alerta],
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL ){

								//En ese caso estamos ante una regla creada anteriormente
								coincide = true;	
	
							}

							//En otro caso, estamos ante una nueva regla
							else {
								
								coincide = false;
								
							}
						}

						//Si no llevaba puerto asociado seguimos comprobando
						else {
							
							//Comprobamos que el puerto destino es "any"
							if (strcmp("any",  	
									contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL){
								
								//Estamos ante una regla ya creada si todo lo anterior es igual		
								coincide = true;
								
							}
								
							else {

								//Estamos ante una nueva regla
								coincide = false;
		
							}
		
						}
		
					}
		
					else {
						//En otro caso, es una nueva regla
						coincide = false;
		
					}		
				}
	
		}

		else {
				
			//En caso contrario estamos ante una nueva regla
			coincide = false;
	
		}
	
	}

	return coincide;
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
	
	//Array auxiliar para ir formando la regla
		//CAMPOS_REGLAS número de palabras que contendrá como máximo una regla
	char *reglas_aux= (char *) malloc (sizeof (char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para recorre el bucle
	int cont_aux_regla;
	//Bandera para detectar la coincidencia en una regla
	bool coincide;

	//Inicializamos
	cont_aux_regla = INICIO;
	coincide = true;

	//Recorremos la estructura de coincidencias que le pasasmos como parámetro
	for (cont_aux_regla=0; cont_aux_regla<contenido_fichero_alerta.numero_lineas; cont_aux_regla++) {
		
		//Abrimos el fichero de las reglas de snort en modo lectura
		fichero_escritura = fopen(nombre_fichero_escritura, "r");
		
		if (fichero_escritura != NULL){
			//Leemos el fichero de reglas ya creadas y almacenamos la info en la estructura
			contenido_del_fichero_reglas = lee_fichero(nombre_fichero_escritura);
			fclose(fichero_escritura);
		}
		
		//Comprobamos si la regla que queremos crear está ya en el fichero o no
		coincide = comprueba_Regla(contenido_del_fichero_reglas, contenido_fichero_alerta, accion, cont_aux_regla);
		
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
				
				//En ese caso añadimos la dirección al campo origen
				strcat(reglas_aux, contenido_fichero_alerta.dir_IP[cont_aux_regla]);
				strcat(reglas_aux, " ");

				//Comprobamos si dicha dirección IP llevaba asociada un puerto
				if (contenido_fichero_alerta.dir_Con_Puerto[cont_aux_regla] == true){
					
					//Si lo llevaba, almacenamos el puerto en la regla
					strcat(reglas_aux, contenido_fichero_alerta.puerto[cont_aux_regla]);	
				
				}
				
				else {
					
					//En caso contrario, añadimos el valor "any"
					strcat(reglas_aux, "any");
				}
			}

			//Si la dir_IP no estaba en el campo origen añadimos los valores por defecto
			else{

				strcat(reglas_aux, "any");
				strcat(reglas_aux, " ");	
				strcat(reglas_aux, "any");
				
			}

			//Añadimos el sentido de la comunicación
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
				else {

					strcat(reglas_aux, "any");
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
	
			//Eliminamos dicho fichero
			system("rm -f sid");
					
			//Lo volvemos a crear y almacenamos el nuevo número
			fichero_sid = fopen("sid", "a+");
			
			if (fichero_sid != NULL){
				fputs(sid, fichero_sid);
				fclose(fichero_sid);
			}
			
			//Almacenamos este sid en la regla que estamos creando
			strcat(reglas_aux, sid);
			//Añadimos el número de revisión, en nuestro caso será siempre 1
			strcat(reglas_aux, "; rev:1;)");
			strcat(reglas_aux, "\n");
			
			//Abrimos el fichero de las reglas y nos situamos al final de este	
			fichero_escritura = fopen(nombre_fichero_escritura, "a+");
			
			//Escribimos la regla que acabamos de crear
			if (fichero_escritura != NULL){
				fputc('\n', fichero_escritura);
				fputs(reglas_aux, fichero_escritura);
				fclose(fichero_escritura);
			}
					
		}		
		
	}

	//Liberamos memoria
	free(reglas_aux);
}	
