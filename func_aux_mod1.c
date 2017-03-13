/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 13-03-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usarán en el primer módulo
-------------------------------------------*/

#include "func_aux_mod1.h"

/*---------------------------------------------------------
	
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	el nombre que vamos a buscar y la línea por la
	que debe empezar a buscar

	Devuelve: Estructura del tipo ESTRUCTURA_REGLA

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

struct ESTRUCTURA_REGLA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, 
	char * nombre_Coincidencia, int linea_inicio){
	
	//Variable para almacenar la línea en la que se ha detectado la coincidencia de DoS
	int linea_coincidencia;
	//Bandera para detectar cuando se ha producido una coincidencia
	bool flag;
	//Bandera para detectar si la red es local
	bool red_local;
	//Bandera para almacenar cuando se detecta una red externa
	bool detectada_red_externa;
	//Estructura que alamcena los datos relativos a las coincidencias
	struct ESTRUCTURA_REGLA contenido_fichero_alerta;
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
	for (cont_aux_frases_fichero=linea_inicio; 
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
						//Almacenamos en el puerto el caracter "\0" para indicar que estamos ante una dirección IP sin puerto
						puerto = strdup ("\0");
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
						//Comprobamos si la dirección IP tiene un puerto asociado	
						if (strcmp(puerto, "\0") != IGUAL){
							
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

				//Alamcenamos la acción que queremos realizar
				contenido_fichero_alerta.accion[numero_de_coincidencia] = strdup("drop");
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

