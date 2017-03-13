/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 27-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usarán en el tercer módulo
-------------------------------------------*/
#include "func_aux_mod3.h"


/*------------------------------------------------------------
	Función que se encarga de leer un fichero de tipo CSV

	Recibe: El nombre del fichero a leer

	Devuelve: Una estructura del tipo CONTENIDO_FICHERO 
				con los datos obtenidos del fichero

	Características:
		Los fichero CSV diferencian sus valores por el caracter
		',' por lo que esto nos marcará el fin de cada palabra
------------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero){
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//String para almacenar la palabra antes de introducirla en la estructura
	char *palabra_aux = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable para contabilizar el número de palabras
	int palabras;
	//Variables para contabilizar el número de frases
	int frases;
	//Variable auxiliar para recorrer el string
	int letra;
	//Variable para el fichero que vamos a leer
	FILE *fichero_lectura;
	//Variable para recorrer el fichero caracter a caracter
	signed char c;
	//Variable para almacenar el caracter que se ha leido anteriormente
	signed char c_ant;


	//Abrimos el fichero
	fichero_lectura = fopen(fichero, "r");

	//Inicializamos
	c_ant = ESPACIO;
	palabras = INICIO;
	frases = INICIO;
	letra = INICIO;
	

	if (fichero_lectura != NULL){

		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF){
			
			//Comprobamos que no superamos el límite
			if (frases < NUM_FRASES){

				// Si estamos ante una línea en blanco
				if ( (c == SALTO_DE_LINEA && c_ant == SALTO_DE_LINEA)){
					
					//Estamos ante una línea vacía
					//Reiniciamos el contador de palabras y el de letras
					palabras = INICIO;
					letra= INICIO;
				}
				
				//Si no estamos ante una línea en blanco
				else  {	

					//Comprobamos que no estamos ante un doble espacio en blanco
					if ( !(c  == ESPACIO && c_ant == ESPACIO) ){
						
						//Nos aseguramos que no es el caracter ',' que se usa para separar palabras
						if (c != COMA){

							//Comprobamos si estamos ante un caracter que no sea '\n' ni ' '
							if (c != SALTO_DE_LINEA && c != ESPACIO ){
								
								//Almacenamos ese caracter	
								palabra_aux[letra] = c;
								//Aumentamos el contador para almacenar el próximo caracter en esa posición
								letra++;

							}
							
							//Si estamos ante un salto de línea
							else if (c == SALTO_DE_LINEA ) {
								
								//Añadimos el terminador
								palabra_aux[letra++] = TERMINADOR;
								
								//Almacenamos la palabra en la estructura
								contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);
								//Almacenamos el número de palabras que tiene esa frase
								contenido_del_fichero.palabras_por_frase[frases] = palabras;
								
								//Aumentamos el número de frases
								frases++;
								
								//Reiniciamos los contadores
								palabras = INICIO;
								letra = INICIO;
						
							}
							
							//Si estamos ante un espcacio
							else if (c == ESPACIO && c_ant != COMA){
								
								//Almacenamos el espacio 
								palabra_aux[letra] = ESPACIO;
								//Aumentamos el contador
								letra++;
										
							}
						}
						//Si estamos leyendo el caracter ',' estamos ante una nueva palabra
						else {

							//Añadimos el terminador
							palabra_aux[letra++] = TERMINADOR;

							//Almacenamos la palabra en la estructura
							contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);

							//Aumentamos el número de palabras leiddas
							palabras++;
							//Reiniciamos el contador	
							letra = INICIO;
						}
					}
				}
			
				//Guardamos el caracter que acabamos de leer
				c_ant = c;
			}
			//En caso contrario indicamos que hemos sobrepasado el número de líneas
			else {

				fprintf(stderr, ERROR_NUM_LINEAS, fichero, NUM_FRASES);
			}
		}

		//Almacenamos el número total de líneas leidas
		contenido_del_fichero.num_frases_fichero = frases;

	
	
        //Cerramos el fichero
        fclose(fichero_lectura);
        
	}

	else {
	
		fprintf(stderr,ERROR_APERTURA_FICHERO, fichero);
	
	}

	
	//Liberamos la memoria
	free(palabra_aux);

	return contenido_del_fichero;
}




/*------------------------------------------------------
	Función que se encarga de extraer del fichero leido
	la información de los SSID

	Devuelve: Estructura del tipo INFO_SSID formada por:
		-- essid[NUM_SSID]: Array que almacena los
					ESSID leidos
		-- bssid[NUM_SSID]: Array que almacena los 
					BSSID leidos
		-- num_ssid: Contador que almacena el número
			total de SSID leidos para poder recorrer
			los arrays en cualquier momento
	Recibe: Estructura del tipo CONTENIDO_FICHERO
------------------------------------------------------*/
struct INFO_SSID procesa_fichero_CSV(struct CONTENIDO_FICHERO contenido_del_fichero){

	//Estructura que será devuelta
	struct INFO_SSID info_ssid;
	//Variable auxiliar para almacenar el número de SSID detectados
	int cont_aux_ssid;
	//Variable auxiliar para recorrer las filas del fichero CSV
	int cont_aux_filas;
	//Variable auxiliar para recorrer las columnas del fichero CSV
	int cont_aux_columnas;
	//Variable para almacenar la columna en la que se encuentra el ESSID
	int pos_essid;
	//Variable para alamcenar la columna donde se encuentra el BSSID
	int pos_bssid;
	//Variable para alamcenar la fila en la que se encuentra la palabra ESSID
	int fil_essid;
	//Variable para almacenar la fila en la que se encuentra la palbra BSSID
	int fil_bssid;
	//Bandera para indicar el momento en el que comienza la info de los clientes
	bool info_clientes;

	//Inicializamos
	cont_aux_ssid = INICIO;
	cont_aux_filas = INICIO;
	cont_aux_columnas = INICIO;
	pos_bssid = INICIO;
	fil_bssid = INICIO;
	pos_essid = INICIO;
	fil_essid = INICIO;
	info_clientes = false;

	//Recorremos la estructura
	for (cont_aux_filas=1; cont_aux_filas< contenido_del_fichero.num_frases_fichero; cont_aux_filas++){
		
		for (cont_aux_columnas=0; cont_aux_columnas<=contenido_del_fichero.palabras_por_frase[cont_aux_filas]; cont_aux_columnas++){
			
			/*--------------------------------------------------
				Al ser un fichero CSV, tiene una estructura
				fija en la que cada valor pertenece a una
				columna, aprovechando esto, vamos a fijar
				los valores de las columnas para obtener
				los que nos interesa
			--------------------------------------------------*/
			//Buscamos la información que ocuparán los ESSID
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], ESSID) == IGUAL){
				pos_essid = cont_aux_columnas;
				fil_essid = cont_aux_filas;
			}

			//Buscamos la posición que ocuparán los BSSID
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], BSSID) == IGUAL){
				pos_bssid = cont_aux_columnas;
				fil_bssid = cont_aux_filas;
			}
			
			//Comprobamos si vamos a pasar a ver la información relativa a los clientes
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], INFO_CLIENTES) == IGUAL){
				
				//Cambiamos el valor de la bandera
				info_clientes = true;
			}

			if (info_clientes == false 
				&& (cont_aux_filas > fil_bssid && cont_aux_filas > fil_essid)){

				if (cont_aux_columnas== pos_bssid){
					info_ssid.bssid[cont_aux_ssid] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas]);
					
				}

				else if (cont_aux_columnas==pos_essid) {
					info_ssid.essid[cont_aux_ssid] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas]);
					cont_aux_ssid++;
				
				}
			}
		}
	}
	info_ssid.num_ssid = cont_aux_ssid;

	return info_ssid;
}


/*------------------------------------------------------------------
	Función que se encarga de detectar la posibilidad de SSID 
	duplicado.

	Devuelve: La estructura del tipo INFO_SSID

	Recibe: Estructura del tipo INFO_SSID, el ssid y la MAC que
	queremos comprobar

	Caracterísitcas:
		Comprobamos los dos casos que se pueden dar para suplantar
		un SSID, mismo ESSID y distinto BSSID o viceversa
------------------------------------------------------------------*/
struct INFO_SSID busca_SSID(struct INFO_SSID info_ssid, char *ssid, char *mac_ssid){

	//Estructura que almacenará los SSID iguales a los que se les pasa por parámetros
	struct INFO_SSID ssid_coincidentes;
	//Contador auxiliar para recorrer la estructura
	int cont_aux_ssid;
	//Contador auxiliar para almacemar el número de coincidencias
	int num_ssid_coincidentes;
	

	//Inicializamos
	cont_aux_ssid = INICIO;
	num_ssid_coincidentes = INICIO;
	

	for (cont_aux_ssid=0; cont_aux_ssid<info_ssid.num_ssid;cont_aux_ssid++){

		
		/*--------------------------------------------
			La primera casuística sería la del 
			la suplantacion del ESSID, para ello
			buscamos si el ESSID coincide y el BSSID
			es distinto
		--------------------------------------------*/
		if ((strcmp(info_ssid.essid[cont_aux_ssid], ssid) == IGUAL) && (strcmp(info_ssid.bssid[cont_aux_ssid], mac_ssid) != IGUAL)) {

			//Almacenamos los datos en la estructura
			ssid_coincidentes.essid[num_ssid_coincidentes] = info_ssid.essid[cont_aux_ssid];
			ssid_coincidentes.bssid[num_ssid_coincidentes] = info_ssid.bssid[cont_aux_ssid];
			ssid_coincidentes.coincide_BSSID[num_ssid_coincidentes] = false;
			num_ssid_coincidentes++;
		}

		/*-------------------------------------------
			El segundo caso sería el de suplantar
			el BSSID, por tanto comprobamos si 
			hemos leido el mismo BSSID y distinto
			ESSID
		-------------------------------------------*/
		else if ((strcmp(info_ssid.essid[cont_aux_ssid], ssid) != IGUAL) && (strcmp(info_ssid.bssid[cont_aux_ssid], mac_ssid) == IGUAL)){
			
			//Almacenamos los datos en la estructura
			ssid_coincidentes.essid[num_ssid_coincidentes] = info_ssid.essid[cont_aux_ssid];
			ssid_coincidentes.bssid[num_ssid_coincidentes] = info_ssid.bssid[cont_aux_ssid];
			ssid_coincidentes.coincide_BSSID[num_ssid_coincidentes] = true;
			num_ssid_coincidentes++;

		}
	}

	//Almacenamos el número de coincidencias que hemos encontrado
	ssid_coincidentes.num_ssid = num_ssid_coincidentes;

	//Devolvemos la estructura
	return ssid_coincidentes;
}


/*----------------------------------------------------------
	Función que se encarga de crear una regla en IPTABLES
	si encuentra un ESSID duplicado y notifica si encuentra
	un BSSID duplicado

	Devuelve: Nada

	Recibe: Una estructura del tipo INFO_SSID

	Características:
		Debe comprobar si la coincidencia es de 
		BSSID o ESSID
			
			ESSID; Obtener el BSSID correspondiente
			y bloquear el tráfico mediante IPTABLES
			
			BSSID; Notifica que ha detectado un BSSID duplicado

----------------------------------------------------------*/


void bloquea_SSID (struct INFO_SSID ssid_coincidentes){

	/*--------------------------------------------
		
	--------------------------------------------*/
	//Variable para recorrer la estructura
	int cont_aux_ssid;
	//String para almacenar el comando que vamos a utilizar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Inicializamos
	cont_aux_ssid = INICIO;

	for (cont_aux_ssid=0;cont_aux_ssid<ssid_coincidentes.num_ssid;cont_aux_ssid++){

	
		//Comprobamos si la coincidencia se ha dado en el ESSID
		if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == false) {

			/*--------------------------------------
				Creamos la regla para cortar todo
				el tráfico entrante de esa dirección
				MAC de esta forma evitamos recibir
				los beacons y así la posibilidad de
				establecer una conexión
			---------------------------------------*/

			strcpy(comando, "sudo iptables -A INPUT -m mac --mac-source ");
			strcat(comando, ssid_coincidentes.bssid[cont_aux_ssid]);
			strcat(comando, " -j DROP");
			printf("%s\n", comando);
			/* DESCOMENTAR
			system(comando);
			*/			
		}

		//En caso contrario la coincidencia será en el BSSID
		else if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == true){

			/*---------------------------------------------------
				Dado que estamos analizando el espectro y las
				comunicaciones en capa dos, no podemos obtener
				la dirección IP del SSID del cuál hemos detectado
				el BSSID duplicado, y tampoco podemos aplicar
				la misma regla que anteriormente ya que nos 
				dejaría sin conexión a nuestro router, luego
				la única acción que podemos tomar es notificar
			---------------------------------------------------*/
			fprintf(stdout, BSSID_DUPLICADO, ssid_coincidentes.essid[cont_aux_ssid], ssid_coincidentes.bssid[cont_aux_ssid]);
		}
	}

	free(comando);
}

/*----------------------------------------------------
	Función que se encarga de capturar los SSID
	existentes en la red WiFi

	Recibe: Nada
	Devuelve: Nada

	Características:
		Se encarga de capturar el tráfico durante
		un tiempo, tras esto para la captura para
		generar el fichero correspondiente.
		Antes de iniciar la captura se asegura de 
		que el fichero donde la vamos a almacenar
		no existía previamente
----------------------------------------------------*/

void escanea_WiFi() {

	//Variable para almacenar el nombre del fichero
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//String para almacenar los comandos auxiliares
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el PID
	char *pid = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el nombre del fichero
	char *nombre_fichero_pid = (char * )malloc(sizeof(char *)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para almacenar el nombre del fichero y la extensi�n
	char *nombre_fichero_completo = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para saber en la línea en la que nos encontramos
	int cont_aux_linea;
	//Variable auxiliar para saber en que palabra nos encontramos
	int cont_aux_palara;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;

	//Inicializamos las variables
	strcpy(nombre_fichero_pid, FICHERO_PID_AIRODUMP);
	cont_aux_linea = INICIO;
	cont_aux_palara = INICIO;



	//Almacenamos el nombre del fichero
	strcpy(nombre_fichero, FICHERO_CAPTURA_WIFI_SIN_EXTENSION);


	//Comprobamos si el fichero que vamos a generar existe
	strcpy(nombre_fichero_completo, nombre_fichero);
	strcat(nombre_fichero_completo, EXTENSION_FICHERO_CAPTURA_WIFI);
	/*-------------------------------------------
		Con la función acces podremos saber si
		el fichero existe o no, el parámetro F_OK
		se le pasa para saber si existe
	-------------------------------------------*/
	if ( access(nombre_fichero_completo, F_OK) != NO_EXISTE) {
		
		/*--------------------------------------------
			En caso de que exista, nos interesa
			eliminarlo ya que airodump no sobreescribe
			los ficheros sino que va añadiendo una 
			terminación numérica y de esta forma
			nos sería imposible seguir por qué fichero
			vamos, así que si lo eliminamos nos 
			aseguramos que siempre lo escribimos con 
			el mismo nombre
		---------------------------------------------*/
		//Eliminamos el fichero para la próxima captura
		strcpy(comando, "sudo rm -rf ");
		strcat(comando, nombre_fichero);
		strcat(comando, "*");
		//system(comando);
	}

	//Creamos el comando para iniciar la captura
	strcpy(comando, "{ sudo airodump-ng mon0 --output-format csv -w ");
	strcat(comando, nombre_fichero);
	strcat(comando, " 2>> output.txt; } &");
	//system(comando);
	
	//Capturamos durante un tiempo
	sleep(TIEMPO_CAPTURA);
	
	//Eliminamos el fichero auxiliar que hemos creado anteriormente
	strcpy(comando, "rm -rf output.txt");
	system(comando);
	
	//Creamos el comando para obtener el PID y redireccionarlo al fichero
	strcpy(comando, "pidof airodump-ng > ");
	strcat(comando, nombre_fichero_pid);
	//Ejecutamos dicho comando
	//system(comando);


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

			//Creamos el comando para parar la captura
        		strcpy(comando, "sudo kill -9 ");
        		strcat(comando, pid);
        		//Ejecutamos dicho comando
        		//system(comando);
		}
	}

	
	//Liberamos memoria
	free(nombre_fichero);
	free(nombre_fichero_pid);
	free(nombre_fichero_completo);
	free(comando);
	free(pid);
}
