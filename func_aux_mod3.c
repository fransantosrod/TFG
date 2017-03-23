/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 27-02-2017
Descripci�n: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usar�n en el tercer m�dulo
-------------------------------------------*/
#include "func_aux_mod3.h"


/*------------------------------------------------------------
	Funci�n que se encarga de leer un fichero de tipo CSV

	Recibe: El nombre del fichero a leer

	Devuelve: Una estructura del tipo CONTENIDO_FICHERO 
				con los datos obtenidos del fichero

	Caracter�sticas:
		Los fichero CSV diferencian sus valores por el caracter
		',' por lo que esto nos marcar� el fin de cada palabra
------------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero_csv(char *fichero){
	
	//Estructura que ser� devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//String para almacenar la palabra antes de introducirla en la estructura
	char *palabra_aux = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Cadena para almacenar el informe de log
	char *log = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable para contabilizar el n�mero de palabras
	int palabras;
	//Variables para contabilizar el n�mero de frases
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
	
	//Si hemos abierto el fichero correctamente
	if (fichero_lectura != NULL){

		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF){
			
			//Comprobamos que no superamos el l�mite
			if (frases < NUM_FRASES){

				// Si estamos ante una l�nea en blanco
				if ( (c == SALTO_DE_LINEA && c_ant == SALTO_DE_LINEA)){
					
					//Reiniciamos el contador de palabras y letras
					palabras = INICIO;
					letra= INICIO;
				}
				
				//Si no estamos ante una l�nea en blanco
				else  {	

					//Comprobamos que no estamos ante un doble espacio en blanco
					if ( !(c  == ESPACIO && c_ant == ESPACIO) ){
						
						//Nos aseguramos que no es el caracter ',' que se usa para separar palabras
						if (c != COMA){

							//Comprobamos si estamos ante un caracter que no sea '\n' ni ' '
							if (c != SALTO_DE_LINEA && c != ESPACIO ){
								
								//Almacenamos ese caracter	
								palabra_aux[letra] = c;
								//Aumentamos el contador para almacenar el pr�ximo caracter en esa posicion
								letra++;

							}
							
							//Si estamos ante un salto de l�nea
							else if (c == SALTO_DE_LINEA ) {
								
								//A�adimos el terminador
								palabra_aux[letra++] = TERMINADOR;
								
								//Almacenamos la palabra en la estructura
								contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);
								//Almacenamos el n�mero de palabras que tiene esa frase
								contenido_del_fichero.palabras_por_frase[frases] = palabras;
								
								//Aumentamos el n�mero de frases
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

							//A�adimos el terminador
							palabra_aux[letra++] = TERMINADOR;

							//Almacenamos la palabra en la estructura
							contenido_del_fichero.contenido_leido_del_fichero[frases][palabras] = strdup(palabra_aux);

							//Aumentamos el n�mero de palabras leiddas
							palabras++;
							//Reiniciamos el contador	
							letra = INICIO;
						}
					}
				}
			
				//Guardamos el caracter que acabamos de leer para la pr�xima iteraci�n
				c_ant = c;
			}
			
			//En caso contrario indicamos que hemos sobrepasado el n�mero de l�neas
			else {

				//Creamos la cadena que nos informar� de la incidencia producida
				sprintf(log, ERROR_NUM_LINEAS, fichero, NUM_FRASES);

				//Registramos dicha incidencia en el fichero de logs
				registra_log(log);
			}
		}

		//Almacenamos el n�mero total de l�neas le�das
		contenido_del_fichero.num_frases_fichero = frases;

	
	
        //Cerramos el fichero
        fclose(fichero_lectura);
        
	}

	else {
		
		//Creamos la cadena que nos informar� de la incidencia producida
		sprintf(log, ERROR_APERTURA_FICHERO, fichero);

		//Registramos dicha incidencia en el fichero de logs
		registra_log(log);
	
	}

	
	//Liberamos la memoria
	free(palabra_aux);
	free(log);

	//Devolvemos la estructura
	return contenido_del_fichero;
}




/*------------------------------------------------------
	Funci�n que se encarga de extraer del fichero le�do
	la informaci�n de los SSID

	Devuelve: Estructura del tipo INFO_SSID formada por:
		-- essid[NUM_SSID]: Array que almacena los
					ESSID le�dos
		-- bssid[NUM_SSID]: Array que almacena los 
					BSSID le�dos
		-- num_ssid: Contador que almacena el n�mero
			total de SSID le�dos para poder recorrer
			los arrays en cualquier momento

	Recibe: Estructura del tipo CONTENIDO_FICHERO
------------------------------------------------------*/
struct INFO_SSID procesa_fichero_CSV(struct CONTENIDO_FICHERO contenido_del_fichero){

	//Estructura que ser� devuelta
	struct INFO_SSID info_ssid;
	//Variable auxiliar para almacenar el n�mero de SSID detectados
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
			
			//Comprobamos si estamos en la posici�n que ocupar� la informaci�n relativa a los ESSID
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], ESSID) == IGUAL){
				
				//Almacenamos el n�mero de fila y columna que ocupan
				pos_essid = cont_aux_columnas;
				fil_essid = cont_aux_filas;
			}

			//Comprobamos si estamos en la posici�n que ocupar� la informaci�n de los BSSID
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], BSSID) == IGUAL){
				
				//Almacenamos la columna y fila que ocupan
				pos_bssid = cont_aux_columnas;
				fil_bssid = cont_aux_filas;
			}
			
			//Comprobamos si vamos a pasar a ver la informaci�n relativa a los clientes
			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas], INFO_CLIENTES) == IGUAL){
				
				//Cambiamos el valor de la bandera
				info_clientes = true;
			}

			//Si no estamos leyendo info de clientes y estamos en una nueva fila
			if (info_clientes == false 
				&& (cont_aux_filas > fil_bssid && cont_aux_filas > fil_essid)){

				//Comprobamos que la columna en la que nos encontramos es donde se almacenar�n los BSSIDs
				if (cont_aux_columnas== pos_bssid){

					//Almacenamos el BSSID
					info_ssid.bssid[cont_aux_ssid] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas]);
					
				}

				//En caso contrario, comprobamos que estamos en la columna donde se almacenar� la informaci�n del ESSID
				else if (cont_aux_columnas==pos_essid) {

					//Almacenamos la informaci�n del ESSID
					info_ssid.essid[cont_aux_ssid] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_filas][cont_aux_columnas]);
					
					//Aumentamos el n�mero de SSIDs detectados
					cont_aux_ssid++;
				
				}
			}
		}
	}

	//Almacenamos el n�mero de SSIDs detectados en la estructura
	info_ssid.num_ssid = cont_aux_ssid;

	//Devolvemos la estructura
	return info_ssid;
}


/*------------------------------------------------------------------
	Funci�n que se encarga de detectar la posibilidad de SSID 
	duplicado.

	Devuelve: La estructura del tipo INFO_SSID

	Recibe: Estructura del tipo INFO_SSID, el ssid y la MAC que
	queremos comprobar

	Caracter�sitcas:
		Comprobamos los dos casos que se pueden dar para suplantar
		un SSID, mismo ESSID y distinto BSSID o viceversa
------------------------------------------------------------------*/
struct INFO_SSID busca_SSID(struct INFO_SSID info_ssid, char *ssid, char *mac_ssid){

	//Estructura que almacenara los SSID iguales a los que se les pasa como par�metros
	struct INFO_SSID ssid_coincidentes;
	//Contador auxiliar para recorrer la estructura
	int cont_aux_ssid;
	//Contador auxiliar para almacemar el n�mero de coincidencias
	int num_ssid_coincidentes;
	

	//Inicializamos
	cont_aux_ssid = INICIO;
	num_ssid_coincidentes = INICIO;
	

	//Recorremos la estructura
	for (cont_aux_ssid=0; cont_aux_ssid<info_ssid.num_ssid;cont_aux_ssid++){

		
		/*--------------------------------------------
			La primera casu�stica ser� la de 
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
			El segundo caso ser� el de suplantar
			el BSSID, por tanto comprobamos si 
			hemos le�do el mismo BSSID y distinto
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

	//Almacenamos el n�mero de coincidencias que hemos encontrado
	ssid_coincidentes.num_ssid = num_ssid_coincidentes;

	//Devolvemos la estructura
	return ssid_coincidentes;
}


/*----------------------------------------------------------
	Funci�n que se encarga de crear una regla en IPTABLES
	si encuentra un ESSID duplicado y notifica si encuentra
	un BSSID duplicado

	Devuelve: Nada

	Recibe: Una estructura del tipo INFO_SSID con las 
		coincidencias detectadas

	Caracter�sticas:
		Debe comprobar si la coincidencia es de 
		BSSID o ESSID
			
			ESSID; Obtener el BSSID correspondiente
			y bloquear el tr�fico mediante IPTABLES
			
			BSSID; Notifica que ha detectado un BSSID duplicado

----------------------------------------------------------*/


void bloquea_SSID (struct INFO_SSID ssid_coincidentes){

	//Variable para recorrer la estructura
	int cont_aux_ssid;
	//String para almacenar el comando que vamos a utilizar
	char *comando = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Cadena para almacenar el informe de log
	char *log = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);

	//Inicializamos
	cont_aux_ssid = INICIO;

	//Recorremos la estructura
	for (cont_aux_ssid=0;cont_aux_ssid<ssid_coincidentes.num_ssid;cont_aux_ssid++){

	
		//Comprobamos si la coincidencia se ha dado en el ESSID
		if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == false) {

			/*--------------------------------------
				Creamos la regla para cortar todo
				el tr�fico entrante de esa direcci�n
				MAC de esta forma evitamos recibir
				los beacons y as� la posibilidad de
				establecer una conexi�n
			---------------------------------------*/

			strcpy(comando, "sudo iptables -A INPUT -m mac --mac-source ");
			strcat(comando, ssid_coincidentes.bssid[cont_aux_ssid]);
			strcat(comando, " -j DROP");
			system(comando);
		}

		//En caso contrario la coincidencia ser� en el BSSID
		else if (ssid_coincidentes.coincide_BSSID[cont_aux_ssid] == true){

			/*---------------------------------------------------
				Dado que estamos analizando el espectro y las
				comunicaciones son en capa dos, no podemos obtener
				la direcci�n IP del SSID del cu�l hemos detectado
				el BSSID duplicado, y tampoco podemos aplicar
				la misma regla que anteriormente ya que nos 
				dejar�a sin conexi�n a nuestro router, luego
				la �nica acci�n que podemos tomar es notificar
			---------------------------------------------------*/
			
			//Creamos la cadena para notificar esto mediante el fichero de logs
			sprintf(log,BSSID_DUPLICADO, ssid_coincidentes.essid[cont_aux_ssid], ssid_coincidentes.bssid[cont_aux_ssid]);
			
			//Registramos la incidencia en el fichero
			registra_log(log);
		}
	}

	//Liberamos la memoria
	free(comando);
	free(log);
}

/*----------------------------------------------------
	Funci�n que se encarga de capturar los SSID
	existentes en la red WiFi

	Recibe: Nada
	Devuelve: Nada

	Caracter�sticas:
		Se encarga de capturar el tr�fico durante
		un tiempo, tras esto para la captura para
		generar el fichero correspondiente.
		Antes de iniciar la captura se asegura de 
		que el fichero donde la vamos a almacenar
		no exist�a previamente
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
	//Variable auxiliar para saber en la l�nea en la que nos encontramos
	int cont_aux_linea;
	//Variable auxiliar para saber en que palabra nos encontramos
	int cont_aux_palara;
	//Estructura que almacenar� los datos relativos al fichero
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
		Con la funci�n acces podemos saber si
		el fichero existe o no, el par�metro F_OK
		se le pasa para saber si existe
	-------------------------------------------*/
	if ( access(nombre_fichero_completo, F_OK) != NO_EXISTE) {
		
		/*--------------------------------------------
			En caso de que exista, nos interesa
			eliminarlo ya que airodump no sobreescribe
			los ficheros sino que va a�adiendo una 
			terminaci�n num�rica y de esta forma
			nos ser�a imposible seguir por qu� fichero
			vamos, as� que si lo eliminamos nos 
			aseguramos que siempre lo escribimos con 
			el mismo nombre
		---------------------------------------------*/
		//Eliminamos el fichero para la pr�xima captura
		strcpy(comando, "sudo rm -rf ");
		strcat(comando, nombre_fichero);
		strcat(comando, "*");
		system(comando);
	}

	//Creamos el comando para iniciar la captura
	strcpy(comando, "{ sudo airodump-ng mon0 --output-format csv -w ");
	strcat(comando, nombre_fichero);
	//Redireccionamos la posible salida de errores a un ficheo auxiliar
	strcat(comando, " 2>> output.txt; } &");
	system(comando);
	
	//Capturamos durante un tiempo
	sleep(TIEMPO_CAPTURA);
	
	//Eliminamos el fichero auxiliar que hemos creado anteriormente
	strcpy(comando, "rm -rf output.txt");
	system(comando);
	
	//Creamos el comando para obtener el PID y redireccionarlo al fichero
	strcpy(comando, "pidof airodump-ng > ");
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

			//Almacenamos el PID en nuestra variable espec�fica
			strcpy(pid,
				contenido_del_fichero.contenido_leido_del_fichero[cont_aux_linea][cont_aux_palara]);

			//Creamos el comando para parar la captura
        		strcpy(comando, "sudo kill -9 ");
        		strcat(comando, pid);
        		//Ejecutamos dicho comando
        		system(comando);
		}
	}

	
	//Liberamos memoria
	free(nombre_fichero);
	free(nombre_fichero_pid);
	free(nombre_fichero_completo);
	free(comando);
	free(pid);
}
