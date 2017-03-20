/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares 
-------------------------------------------*/

#include "func_aux.h"
#include <sys/stat.h>
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
	signed char c;
	//Variable para almacenar el valor del caracter anterior
	signed char c_ant;
	
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
	//Variable para almacenar la dirección del semáforo al abrirlo
	sem_t *semaforo_lectura;

	//Array auxiliar para almacenar cada palabra de la frase
		//NUM_CARACTERES_PALABRA caracteres por palabras
	char *palabras_frase = (char *)malloc (sizeof(char)*NUM_CARACTERES_PALABRA);
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;

	/*------------------------------------
		Abrimos el semáforo pasandole 
		los siguientes argumentos a la 
		función:
			-- SEM_LECTURA: Nombre del
				semáforo
			-- INICIO: Bandera para 
			indicar que el semáforo ya
			existe(valor=0)
	------------------------------------*/
	semaforo_lectura = sem_open(SEM_LECTURA, INICIO);
	
	if (semaforo_lectura != NULL){

		//Bajamos el semáforo
		sem_wait(semaforo_lectura);
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
					if (c!= ESPACIO && c!=SALTO_DE_LINEA && c!= TABULADOR){
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
						palabras_frase[letra++] = TERMINADOR;
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
					
					else if ((c == ESPACIO || c == TABULADOR) && letra >0  ){
					
						//Almacenamos la palabra de la frase deseada en nuestra estructura
						palabras_frase[letra++] = TERMINADOR;
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
					fprintf(stderr, ERROR_NUM_LINEAS, nombre_fichero, NUM_FRASES);
				}
			}
			fclose(fichero_lectura);
		}

		else 
			fprintf(stderr,ERROR_APERTURA_FICHERO, nombre_fichero);
		
		//Subimos el semáforo
		sem_post(semaforo_lectura);
		
		//Cerramos el semáforo
		sem_close(semaforo_lectura);	
		
		//Rellenamos la estructura para poder usarla en otro punto del codigo
		contenido_del_fichero.num_frases_fichero = frase;
	}

	else {
		fprintf(stderr, ERROR_APERTURA_SEMAFORO, SEM_LECTURA);
	}
	//Liberamos la memoria
	free(palabras_frase);
	
	return contenido_del_fichero;
}




/*-----------------------------------------------------
	Función que se encarga de buscar en el fichero 
	de reglas donde se quiera insertar si existe 
	una igual para evitar introducir reglas duplicadas

	Recibe: Estructura del tipo CONTENIDO_FICHERO,
	estructura del tipo ESTRUCTURA_REGLA,
	y la posición dentro de la estructura ESTRUCTURA_REGLA
	donde se encuentra leyendo la info relativa a la nueva
	regla

	Devuelve: Un entero
		-->0: Ya hay una regla igual. Es la línea que
		ocupa en el fichero donde están las reglas
		-- -1: No la hay

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

int busca_Regla(struct CONTENIDO_FICHERO contenido_del_fichero_reglas, struct ESTRUCTURA_REGLA contenido_fichero_alerta, 
	int pos_dentro_estruct_regla) {

	
	//Bandera para detectar la coincidencia en una regla
	int coincide;
	//Variable auxiliar para recorrer la tabla devuelta al leer el fichero
	int cont_aux_linea;
	
	//Inicializamos las variables
	cont_aux_linea = INICIO;
	coincide = NO_COINCIDE;
	

	//Recorremos la estructura leida del fichero de reglas para buscar coincidencias con la que vamos a introducir
	//Estaremos aquí mientras que no se haya encontrado ninguna coincidencia
	for (cont_aux_linea=0; 
			cont_aux_linea<contenido_del_fichero_reglas.num_frases_fichero && coincide == NO_COINCIDE; 
			cont_aux_linea++){

		//Comprobamos si los valores a introducir coinciden con los que ya están
		/*-----------------------------------------------------------------
			Aprovechando la estructura de las reglas que es la siguiente:
			accion protocolo dir_IP_orig puerto_orig ->
				dir_IP_dest puerto_dest
			Estos valores serán los que comprobaremos
		-----------------------------------------------------------------*/

		//Comprobamos conjuntamente si la pareja acción protocolo son iguales
		if ((strcmp(contenido_fichero_alerta.accion[pos_dentro_estruct_regla], contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_ACCION]) == IGUAL) &&
		 	(strcmp(contenido_fichero_alerta.protocolo[pos_dentro_estruct_regla], 
				contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PROTOCOLO]) == IGUAL )){
				
				//Comprobamos si la dirección IP estaba en el campo origen	
				if (contenido_fichero_alerta.dir_en_origen[pos_dentro_estruct_regla] == true){

					//En ese caso, comprobamos que coincida la dir_IP con la origen de la regla y la destino con "$HOME_NET"
					if (strcmp (contenido_fichero_alerta.dir_IP[pos_dentro_estruct_regla],
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL &&
						strcmp("$HOME_NET", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST] )== IGUAL){

						//Comprobamos si la dir_IP tenía asociada un puerto
						if (contenido_fichero_alerta.dir_Con_Puerto[pos_dentro_estruct_regla] == true){

							//Si es así comprobamos si el puerto coincide
							if (strcmp (contenido_fichero_alerta.puerto[pos_dentro_estruct_regla],
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL ){

								//Si todo lo anterior coincide, la regla ya existía
								coincide = cont_aux_linea;
							}

							else {
								//En caso contrario, estamos ante una nueva regla
								coincide = NO_COINCIDE;
							}
						}

						else {
								
							//Si no tenía asociada ningún puerto, comprobamos que es "any"
							if (strcmp("any", 
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_ORIG]) == IGUAL) {
								
								//Si coincide, estamos ante una regla ya existente
								coincide = cont_aux_linea;
	
							}
								
							else {

								//En caso de que no coincida, es una nueva regla
								coincide = NO_COINCIDE;
	
							}
						}
					}

					else {
						
						//En caso contrario, estamos ante una nueva regla
						coincide = NO_COINCIDE;
	
					}
	
				}
				
				//Si la dirección IP no estaba en el campo origen
				else { 
					
					//Comprobamos que el campo origen sea "$HOME_NET" y el destno la dir_IP de la regla que queremos crear
					if (strcmp("$HOME_NET", contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_ORIG]) == IGUAL &&
						strcmp (contenido_fichero_alerta.dir_IP[pos_dentro_estruct_regla],
							contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_DIR_IP_DEST]) == IGUAL ){

						//Comprobamos si esa dirección llevaba asociada un puerto
						if (contenido_fichero_alerta.dir_Con_Puerto[pos_dentro_estruct_regla] == true){

							//Si lo llevaba asociado, comprobamos si el puerto coincide
							if (strcmp (contenido_fichero_alerta.puerto[pos_dentro_estruct_regla],
								contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL ){

								//En ese caso estamos ante una regla creada anteriormente
								coincide = cont_aux_linea;	
	
							}

							//En otro caso, estamos ante una nueva regla
							else {
								
								coincide = NO_COINCIDE;
								
							}
						}

						//Si no llevaba puerto asociado seguimos comprobando
						else {
							
							//Comprobamos que el puerto destino es "any"
							if (strcmp("any",  	
									contenido_del_fichero_reglas.contenido_leido_del_fichero[cont_aux_linea][POS_PUERTO_DEST]) == IGUAL){
								
								//Estamos ante una regla ya creada si todo lo anterior es igual		
								coincide = cont_aux_linea;
								
							}
								
							else {

								//Estamos ante una nueva regla
								coincide = NO_COINCIDE;
		
							}
		
						}
		
					}
		
					else {
						//En otro caso, es una nueva regla
						coincide = NO_COINCIDE;
		
					}		
				}
	
		}

		else {
				
			//En caso contrario estamos ante una nueva regla
			coincide = NO_COINCIDE;
	
		}
	
	}

	return coincide;
}


/*--------------------------------------------------------------
	Función que se encarga de crear las reglas a partir de la 
	estructura y la acción a tomar,
	
	Recibe: El nombre del fichero donde estan  almacenadas las 
		reglas , una estructura del tipo ESTRUCTURA_REGLA
		y un string para añadir informacion a la regla
	
	Devuelve: Booleano indicando si se ha escrito la regla

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

bool crea_y_escribe_regla(char *nombre_fichero_escritura, struct ESTRUCTURA_REGLA contenido_fichero_alerta, char *info_extra){
	
	//Variable a devolver que indica si se ha creado la regla
	bool regla_creada;
	//Variable para el fichero donde almacenaremos el sid por el que vamos
	FILE *fichero_sid;
	//Variable auxiliar para la lectura del fichero donde almacenaremos el sid
	signed char c;
	//Array donde almacenaremos el número de sid leido
	char sid[NUM_DIG_SID];
	//Variable auxiliar para indicar la posición de la tabla donde nos encontramos
	int pos_sid;

	//Variable para almacenar el fichero de las reglas, de donde leeremos y escribiremos
	FILE *fichero_escritura;	
	//Estructura que almacenará los datos relativos al fichero de las reglas
	struct CONTENIDO_FICHERO contenido_del_fichero_reglas;
	
	//String auxiliar para ir formando la regla
	char *reglas_aux= (char *) malloc (sizeof (char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para recorre el bucle
	int cont_aux_regla;
	//Bandera para detectar la coincidencia en una regla
	int coincide;
	
	//Variable para almacenar la dirección del semáforo al abrirlo
	sem_t *semaforo_reglas_snort;
	sem_t *semaforo_sid;


	//Abrimos el semáforo
	semaforo_reglas_snort = sem_open(SEM_REGLAS_SNORT, INICIO);
	semaforo_sid = sem_open(SEM_SID, INICIO);
	//Inicializamos
	cont_aux_regla = INICIO;
	coincide = INICIO;
	regla_creada = false;

	//Comprobamos que se han abierto correctamente
	if (semaforo_reglas_snort != NULL && semaforo_sid != NULL) {

		//Recorremos la estructura de coincidencias que le pasasmos como parámetro
		for (cont_aux_regla=0; cont_aux_regla<contenido_fichero_alerta.numero_lineas; cont_aux_regla++) {
			
			//Leemos el fichero de reglas ya creadas y almacenamos la info en la estructura
			contenido_del_fichero_reglas = lee_fichero(nombre_fichero_escritura);

			//Comprobamos si la regla que queremos crear está ya en el fichero o no
			coincide = busca_Regla(contenido_del_fichero_reglas, contenido_fichero_alerta, cont_aux_regla);
			

			//En el caso en el que no encontramos ninguna coincidencia en el fichero
			if (coincide == NO_COINCIDE){
					
				//Comenzamos añadiendo la acción que queremos ejecutar en esa regla
				strcpy(reglas_aux,contenido_fichero_alerta.accion[cont_aux_regla]);
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

					strcat(reglas_aux, "$HOME_NET");
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

					strcat(reglas_aux, "$HOME_NET");
					strcat(reglas_aux, " ");
					strcat(reglas_aux, "any");
				}
						
				strcat(reglas_aux, " ");
				
				//Añadimos los campos opcionales como mensaje de información
				/*----------------------------------------------
					El mensaje de info tendrá la estructura de:
						accion + protocolo
				-----------------------------------------------*/
				strcat(reglas_aux, "(msg:\"");
				strcat(reglas_aux, info_extra);
				strcat(reglas_aux, ": ");
				strcat(reglas_aux,contenido_fichero_alerta.accion[cont_aux_regla]);
				strcat(reglas_aux, " ");
				strcat(reglas_aux, contenido_fichero_alerta.protocolo[cont_aux_regla]);
				strcat(reglas_aux, "\"; sid:");

				//Abrimos el fichero donde almacenamos el sid
				/*-----------------------------------------------
					El sid lo debemos almacenar en un fichero
					ya que debe ser único dentro de snort.local
					de esta forma siempre tenemos la referencia
					del último que hemos usado para la última
					regla introducida
				-----------------------------------------------*/
				//Bajamos el semáforo
				sem_wait(semaforo_sid);

				//Leemos el fichero
				fichero_sid = fopen(FICHERO_SID, "r");

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
					fprintf(stderr, ERROR_APERTURA_FICHERO, FICHERO_SID);	
				
				
				//Lo volvemos a crear y almacenamos el nuevo número
				fichero_sid = fopen(FICHERO_SID, "w");
				
				if (fichero_sid != NULL){
					fputs(sid, fichero_sid);
					fclose(fichero_sid);
				}
				
				//Subimos el semáforo
				sem_post(semaforo_sid);

				
			
				//Almacenamos este sid en la regla que estamos creando
				strcat(reglas_aux, sid);
				//Añadimos el número de revisión, en nuestro caso será siempre 1
				strcat(reglas_aux, "; rev:1;)");
				strcat(reglas_aux, "\n");
				
				//Bajamos el semáforo
				sem_wait(semaforo_reglas_snort);

				//Abrimos el fichero de las reglas y nos situamos al final de este	
				fichero_escritura = fopen(nombre_fichero_escritura, "a+");
				
				//Escribimos la regla que acabamos de crear
				if (fichero_escritura != NULL){
					fputc('\n', fichero_escritura);
					fputs(reglas_aux, fichero_escritura);
					fclose(fichero_escritura);
					regla_creada = true;
				}
				//Subimos el semáforo
				sem_post(semaforo_reglas_snort);

						
			}		
		}
		//Cerramos el semáforo
		sem_close(semaforo_reglas_snort);
		//Cerramos el semáforo
		sem_close(semaforo_sid);		
	}

	//En caso contrario
	else {
		
		//Comprobamos cuál es el que no se ha abierto correctamente
		
		if (semaforo_reglas_snort != NULL) {
			
			sem_close(semaforo_reglas_snort);
			fprintf(stderr, ERROR_APERTURA_SEMAFORO, SEM_SID);
		}

		else if (semaforo_sid != NULL) {

			sem_close(semaforo_sid);
			fprintf(stderr, ERROR_APERTURA_SEMAFORO, SEM_REGLAS_SNORT);
		}

		else {
			fprintf(stderr, ERROR_APERTURA_VARIOS_SEMAFOROS, SEM_SID, SEM_REGLAS_SNORT);
		}
	}

	//Liberamos memoria
	free(reglas_aux);
	
return regla_creada;
}	




/*---------------------------------------------
	Función para que Snort vuelva a leer
	las reglas nuevas que se han introducido

	Recibe: Nada
	Devuelve: Nada

	Caracteristicas:
		Obtiene el PID del proceso que almacena
		en un fichero, posteriormente ese 
		fichero se lee y se ejecuta la orden
		de recarga para ese PID
---------------------------------------------*/
void recarga_Snort () {

	//Variable auxiliar para almacenar el PID
	char *pid = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el nombre del fichero
	char *nombre_fichero_pid = (char * )malloc(sizeof(char *)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el comando que vamos a ejecutar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variable auxiliar para saber en la línea en la que nos encontramos
	int cont_aux_linea;
	//Variable auxiliar para saber en que palabra nos encontramos
	int cont_aux_palara;
	//Variable para almacenar la dirección del semáforo al abrirlo
	sem_t *semaforo_snort;
	
	/*------------------------------------
		Abrimos el semáforo pasandole 
		los siguientes argumentos a la 
		función:
			-- SEM_SNORT: Nombre del
				semáforo
			-- INICIO: Bandera para 
			indicar que el semáforo ya
			existe(valor=0)
	------------------------------------*/
	semaforo_snort = sem_open(SEM_SNORT, INICIO);

	//Comprobamos que hemos abierto correctamente los semáforos
	if (semaforo_snort != NULL) {	

		//Bajamos el semáforo
		sem_wait(semaforo_snort);
		
		//Inicializamos las variables
		strcpy(nombre_fichero_pid, FICHERO_PID_SNORT);
		cont_aux_linea = INICIO;
		cont_aux_palara = INICIO;

		//Creamos el comando para obtener el PID y redireccionarlo al fichero
		strcpy(comando, "pidof snort > ");
		strcat(comando, nombre_fichero_pid);
		//Ejecutamos dicho comando
		system(comando);

		//Leemos el fichero
		contenido_del_fichero = lee_fichero(nombre_fichero_pid);

		//Recorremos el array devuelto para obtener el PID
		for (cont_aux_linea=0;
				cont_aux_linea < contenido_del_fichero.num_frases_fichero; 
				cont_aux_linea++){
			
			for (cont_aux_palara=0;
				cont_aux_palara<=contenido_del_fichero.palabras_por_frase[cont_aux_linea];
				cont_aux_palara++) {

				//Almacenamos el PID en nuestra variable específica
				strcpy(pid,
					contenido_del_fichero.contenido_leido_del_fichero[cont_aux_linea][cont_aux_palara]);

			}
		}

		//Creamos el comando para recargar Snort
		strcpy(comando, "sudo kill -SIGHUP ");
		strcat(comando, pid);
		//Ejecutamos dicho comando
		system(comando);
		
		//Subimos el semáforo
		sem_post(semaforo_snort);
		
		//Cerramos el semáforo
		sem_close(semaforo_snort);
	
	}
	//En caso de que uno de los semáforo no se abra correctamente
	else {
		
		fprintf(stderr, ERROR_APERTURA_SEMAFORO, SEM_SNORT);	
	}
		
	//Liberamos la memoria
	free(nombre_fichero_pid);
	free(comando);
	free(pid);
}



/*---------------------------------------------------
	Función que se encarga de vaciar un fichero
	que está en el límite de líneas que puede
	leer la función que se encarga de ello

	Devuelve: string con el nombre del fichero creado

	Recibe: El nombre del fichero

	Caracteristicas:
		Para vaciar el fichero, crea uno nuevo con
		el contenido que se desea eliminar 
		añadiendo al final del nombre la fecha en
		la que se ha realizado para tenerlo siempre
		almacenado
---------------------------------------------------*/

char *vacia_fichero(char *nombre_fichero){

	//Variable auxiliar para almacenar el instante en el que se cambia el fichero
	time_t tiempo_cambio_fichero = time(NULL);
	//Array para almacenar la fecha
	char t_cambio_fichero[TAM_FECHA];
	//Estructura para almacenar la fecha y hora en el momento deseado
	struct tm *tm;
	//Variable para almacenar el comando que vamos a ejecutar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);

			


	//Obtenemos el instante en el que lo hemos detectado
	tm = localtime(&tiempo_cambio_fichero);
	/*-------------------------------------------------
		La estructura con la que se almacena el nuevo
		fichero es la siguiente:
			nombre_fichero-dia-mes-año_hora:min:seg
	-------------------------------------------------*/
	strftime(t_cambio_fichero, sizeof(t_cambio_fichero), "-%d-%m-%Y_%H:%M:%S", tm);
			
	//Almacecenamos el fichero actual en una copia para no perderlo
	strcpy(comando, "cp -f ");
	strcat(comando, nombre_fichero);
	strcat(comando, " ");
		
	//La copia estará formada por el nombre y la fecha anteriormente obtenida
	strcat(comando, nombre_fichero);
	strcat(comando, t_cambio_fichero);
	system(comando);
		
	//Eliminamos el fichero que está apunto de sobrecargarse
	strcpy(comando, "rm -f ");
	strcat(comando, nombre_fichero);
	system(comando);
		
	//Creamos uno nuevo vacío
	strcpy(comando, "touch -f ");
	strcat(comando, nombre_fichero);
	system(comando);
	
	strcpy(comando, nombre_fichero);
	strcat(comando, t_cambio_fichero);

	return comando;		
	//Liberamos la memoria
	free(comando);
}


/*--------------------------------------------------------
	Función que se encarga de eliminar una regla
	del fichero de reglas de Snort

	Recibe: El fichero del que se quiere eliminar
	las reglas, la estructura del tipo ESTRUCTURA_REGLA
	que contiene la información de la regla que se
	desea borrar y la posición dentro de la 
	estructura

	Devuelve: Nada

	Caracteristicas:
		En primer lugar lee el fichero del que se 
		quieren borrar sus líneas para tener almacenada
		una copia, tras esto se busca la línea donde
		está la regla que queremos elminiar y tras esto
		abre el fichero vacio y lo reescribe con los
		datos que almacenó al principio saltandose la 
		línea que deseamos eliminar

		Tendremos la opción de pasar directamente la 
		línea que queremos eliminar, para ello,
		pasaremos la estructura vacía y en el tercer
		parámetro la línea que queremos eliminar
-------------------------------------------------------*/

void elimina_Regla(char *nombre_fichero_borrar,struct ESTRUCTURA_REGLA informacion_regla, 
	int pos_dentro_estruct_regla){

	//Variable para almacenar el fichero de donde vamos a borrar
	FILE *fichero_borrar;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;
	//Variable auxiliar para almacenar la regla que vamos a sobreescribir
	char *regla_aux = (char *)malloc(sizeof(char) *NUM_CARACTERES_REGLAS);
	//Variable para almacenar la línea en la que hemos encontrado la regla
	int linea_regla;
	//Variable para almacenar la dirección del semáforo al abrirlo
	sem_t *semaforo_reglas_snort;
	
	//Inicializamos las variables
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;
	linea_regla = INICIO;

	//Abrimos el semáforo
	semaforo_reglas_snort = sem_open(SEM_REGLAS_SNORT, INICIO);

	if (semaforo_reglas_snort != NULL) {
		
		//Bajamos el semáforo
		sem_wait(semaforo_reglas_snort);

		//Leemos el fichero del que queremos borrar la regla
		contenido_del_fichero = lee_fichero(nombre_fichero_borrar);	

		//Comprobamos si la estructura está vacía
		if (informacion_regla.numero_lineas > INICIO) {
			//Buscamo dicha regla en el fichero
			linea_regla = busca_Regla(contenido_del_fichero, informacion_regla, pos_dentro_estruct_regla);
		
		}
		/*-------------------------------------------
			En caso de estar vacía, el tercer 
			parámetro nos indica la línea que 
			deseamos eliminar
		--------------------------------------------*/
		else {
			
			linea_regla = pos_dentro_estruct_regla;
		}

		//Si hemos encontrado la regla
		if (linea_regla >= INICIO){

			//Abrimos el fichero vacio para volver a escribirlo
			fichero_borrar = fopen(nombre_fichero_borrar, "w");
			
			//Recorremos el array que rellenamos cuando lo leimos por primera vez
			for (cont_aux_frases_fichero=0;
			cont_aux_frases_fichero < contenido_del_fichero.num_frases_fichero;
			cont_aux_frases_fichero++){
				
				//Comprobamos que la línea que vamos a escribir no es la que queremos eliminar
				if (cont_aux_frases_fichero != linea_regla) {

					for(cont_aux_palabras_fichero =0;
						cont_aux_palabras_fichero <= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero];
						cont_aux_palabras_fichero++) {

						//Si estamos ante la primera palabra de la frase
						if (cont_aux_palabras_fichero == 0){
							//Copiamos para iniciar una nueva regla
							strcpy(regla_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
							
						}
						//Si no encontramos ante otra palabra
						else {
							//Concatenamos las palabras para seguir creando la regla
							strcat(regla_aux, " ");
							strcat(regla_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
							
						}

					}
				//Añadimos un salto de línea
				strcat(regla_aux, "\n");
				//Escribimos la regla en el fichero
				fputs(regla_aux, fichero_borrar);
				}
			}
			//Cerramos el fichero
			fclose(fichero_borrar);
		}
	
		//Subimos el semáforo
		sem_post(semaforo_reglas_snort);

		//Cerramos el semáforo
		sem_close(semaforo_reglas_snort);
	}
	else {

		fprintf(stderr, ERROR_APERTURA_SEMAFORO, SEM_REGLAS_SNORT);
	}

	//Liberamos memoria
	free(regla_aux);
}

/*----------------------------------------------------
	Función que se encarga de crear un semáforo

	Recibe: Nombre del semaforo a crear

	Devuelve: Nada
----------------------------------------------------*/

void crea_semaforo(char *nombre_semaforo){

	//Variable auxiliar para almacenar el valor inicial del semáforo
	int valor_sem;
	//Variable que almacenará la dirección del semáforo una vez que se cree
	sem_t *semaforo;

	//Inicializamos
	valor_sem = INICIO_SEM;

	/*-----------------------------------------------
		Creamos el semáforo pasando los siguientes
		parámetros:
			-- nombre_semaforo: Nombre que tendrá
				el semáforo a crear

			--O_CREAT: Indicamos que lo queremos crear

			--666: Permisos que tendrá el semáforo

			--valor_sem: Valor inicial del semáforo
	-----------------------------------------------*/
	/*-----------------------------------------------
		Cambiamos los valores por defecto de umask
		a "000" de esta forma, a los semáforos
		se le asignarán excatamente los valores
		que se le pasan en la función
	-----------------------------------------------*/
	umask(000);
	semaforo = sem_open(nombre_semaforo, O_CREAT, 0666, valor_sem);
	/*-----------------------------------------------
		Tras crear los semáforos, volvemos a 
		asignar a umask los valores por defectos
		para las próxima ocasión en las que se
		cree cualquier fichero este tenga los valores
		por defecto
	-----------------------------------------------*/
	umask(022);
	if (semaforo == NULL) {

		fprintf(stderr, ERROR_CREACION_SEMAFORO, nombre_semaforo);
	}
}


/*---------------------------------------------
	Función que se encarga de eliminar un
	semáforo que fue creado previamente

	Recibe: El nombre del semáforo a eliminar

	Devuelve: Nada
---------------------------------------------*/
void elimina_semaforo(char *nombre_semaforo) {

	//Variable auxiliar para almacenar el valor devuelto a la hora de eliminar el semáfoross
	int valor_devuelto;

	//Inicializamos
	valor_devuelto = INICIO;

	//Eliminamos el semáforo
	valor_devuelto = sem_unlink(nombre_semaforo);

	//Comprobamos que se eliminó correctamente
	if (valor_devuelto != INICIO) {
		fprintf(stderr, ERROR_CERRAR_SEMAFORO, nombre_semaforo);
	}

}