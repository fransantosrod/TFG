/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 21-02-2017
Descripción: Fichero que contiene las 
implementaciones de las funciones auxiliares
que se usarán en el segundo módulo
-------------------------------------------*/

#include "func_aux_mod2.h"

/*----------------------------------------------------------
	Función que se encarga de leer un fichero y buscar 
	la palabra deseada para almacenar información 

	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	y la línea en la que debe empezar a buscar

	Devuelve: Una estructura del tipo ESTRUCTURA_REGLA

	Caracteristicas:
		Función pensada para leer el fichero de la
		herramienta arpwatch para buscar en él posibles
		conflictos de direcciones cuando se está realizando
		un ataque MITM
----------------------------------------------------------*/

struct ESTRUCTURA_REGLA busca_CAMBIO_EN_MAC(struct CONTENIDO_FICHERO contenido_del_fichero, int linea_inicio) {

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
	for (cont_aux_frases=linea_inicio; 
		cont_aux_frases < contenido_del_fichero.num_frases_fichero; 
		cont_aux_frases++){
		
		/*------------------------------------------------------
			En el fichero arpwatch.log sólo vamos a encontrar
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

	
			//Comprobamos si la palabra actual es la que estamos buscando y hemos detectado un cambio de MAC para una misma dir_IP 
			if (flag_cambio_MAC == false && strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras],
				CAMBIO_EN_MAC) == IGUAL){

				//Almacenamos la línea en la que hemos encontrado la coincidencia
				linea_coincidencia = cont_aux_frases;
				//Almacenamos la palabra en la que hemos encontrado la coincidencia
				palabra_coincidencia = cont_aux_palabras;
				//Cambiamos el valor de la bandera para indicar que hemos encontrado una coincidencia
				flag_cambio_MAC = true;
			}

			/*----------------------------------------------------------------
				Detectamos si ARPWATCH nos ha notificado de un cambio
				en la dirección MAC para una IP que tenía almacenada 
				anteriormente
			----------------------------------------------------------------*/
			//Comprobamos si hemos detectado una coincidencia y la palabra en la que nos encontramos es la relativa a la dirección IP
			if ( flag_cambio_MAC == true && cont_aux_palabras == palabra_coincidencia + POS_DIR_IP){
				
				//Almacenamos la dirección IP
				informacion_regla.dir_IP[num_reglas] = 
					strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases][cont_aux_palabras]);
				
				//Indicamos que la dirección IP no lleva asociado ningún puerto
				informacion_regla.dir_Con_Puerto[num_reglas] = false;

				//Indicamos que queremos que la dirección IP vaya en destino
				informacion_regla.dir_en_origen[num_reglas] = false;

				//Indicamos que el protocolo sea IP para que cubra a todos
				informacion_regla.protocolo[num_reglas] = strdup(IP);
				/*-----------------------------------------------
					En este caso la acción que vamos a tomar
					es la de "alert" ya que esto no conlleva
					gran peligro, pero puede ser útil saberlo
				-----------------------------------------------*/
				//Almacenamos la acción que queremos realizar
				informacion_regla.accion[num_reglas] = strdup("alert");
				//Aumentamos el número de reglas
				num_reglas++;
			
			}
/*-------------------------------------------
	Si en un futuro queremos almacenar
	la dirección MAC e IP de cada cambio
	bastará con descomentar este apartado
-------------------------------------------*/

/*
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
				/*-------------------------------------------------
					Para este caso la acción que vamos a tomar es
					la de "drop" ya que esto puede ser un ataque
					MITM y nos interesa cortarlo
				-------------------------------------------------*/
				//Almacenamos la acción que queremos realizar
				informacion_regla.accion[num_reglas] = strdup("drop");
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

	//Devolvemos la estructura
	return informacion_regla;
}




/*-------------------------------------------------------------
	Función que se encarga de registrar las reglas ante los 
	ataques MITM que se puedan producir para posteriormente
	poder borrarlas ya que estas serán temporales

	Recibe: Estructura del tipo ESTRUCTURA_REGLA con la info
	de las relglas que queremos crear

	Devuelve: Nada

	Caracteristicas:
		Esta función en primer lugar, abre el fichero de 
		reglas de Snort y obtiene las que se han creado
		tras esto, comprueba si la info que tenemos
		de la nueva que vamos a registrar es la relativa a
		una regla ya existente, si esto ocurre no 
		registraremos nada ya que se sobreentiende que se
		registro anteriormente
-------------------------------------------------------------*/
void registra_Regla(struct ESTRUCTURA_REGLA informacion_regla){

	//Fichero para almacenar la regla y la hora de creación
	FILE *fichero_registro;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//String auxiliar para almacenar la información que vamos a escribir
	char *info_reglas = (char *)malloc(sizeof(char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para almacenar el instante en el que se cambia el fichero
	time_t tiempo_creacion_regla = time(NULL);
	//Array para almacenar la fecha
	char t_creacion_regla[TAM_FECHA];
	//Variable auxiliar para recorrer la estructura
	int cont_aux_reglas;
	//Variable para almacenar si ya hay una regla igual a la que queremos introducir
	int coincidencia_regla;
	//Estructura para almacenar hora en el momento deseado
	struct tm *tm;
	
	//Inicializamos las variables
	fichero_registro = fopen(FICHERO_REGISTRO_REGLAS_MITM, "a+");
	cont_aux_reglas = INICIO;
	coincidencia_regla = INICIO;

	//Recorremos la estructura donde tenemos la info de las reglas
	for (cont_aux_reglas=0;
		cont_aux_reglas< informacion_regla.numero_lineas;
		cont_aux_reglas++){

		//Leemos el fichero de las reglas que ya hemos creado
		contenido_del_fichero = lee_fichero(FICHERO_REGLAS_SNORT);
		
		/*--------------------------------------------------------
			El siguiente paso es necesario realizarlo ya que
			esto se realiza ante de escribir las reglas luego
			no sabemos si la info que vamos a registrar estaba
			registrada anteriormente y de esta forma evitamos que
			estén duplicados los registros
		---------------------------------------------------------*/
		//Miramos a ver si existe alguna igual a la que vamos a registrar
		coincidencia_regla = busca_Regla(contenido_del_fichero, informacion_regla, cont_aux_reglas);
		
		//Si la regla no estaba creada anteriormente
		if (coincidencia_regla == NO_COINCIDE){

			//Obtenemos el instante en el que lo hemos detectado
			tm = localtime(&tiempo_creacion_regla);
			
			//Nos interesa obtener la hora y los minutos de creación de la regla
			strftime(t_creacion_regla, sizeof(t_creacion_regla), "%H%M", tm);
			strcpy(info_reglas, t_creacion_regla);
			strcat(info_reglas, " ");

			//Añadimos la información de la reglas

			//Acción a tomar
			strcat(info_reglas, informacion_regla.accion[cont_aux_reglas]);
			strcat(info_reglas, " ");


			//Protocolo
			strcat(info_reglas, informacion_regla.protocolo[cont_aux_reglas]);
			strcat(info_reglas, " ");

			//IP
			strcat(info_reglas, informacion_regla.dir_IP[cont_aux_reglas]);
			strcat(info_reglas, " ");

			//Posición de la dirección IP
			if (informacion_regla.dir_en_origen[cont_aux_reglas] == true){
				strcat(info_reglas, "true");
			}
			else {
				strcat(info_reglas, "false");
			}
			strcat(info_reglas, " ");


			
			/*-----------------------------------------------
				Sabemos de antemano que los ataques MITM no
				llevan ningún puerto asociado
			-----------------------------------------------*/
			//Asociamos la bandera al puerto
			strcat(info_reglas, "false");
			strcat(info_reglas, "\n");

			//Escribimos en el fichero
			fputs(info_reglas, fichero_registro);
		}
	}
		
	//Cerramos el fichero
	fclose(fichero_registro);
	
	

	//Liberamos memoria
	free(info_reglas);
}


/*--------------------------------------------------------
	Función que se encarga de detectar cuando una regla
	sobrepasa un tiempo dado, en este caso una hora

	Recibe: El nombre del fichero

	Devuelve: Nada

	Características:
		Lo primero que realiza es la lectura del fichero
		tras esto, toma la hora actual y comienza a 
		recorrer el array del fichero que hemos leido 
		para comparar los tiempos en los que se realizó
		la creación de la regla y la hora actual
		Si este tiempo es mayor a una hora, procedemos 
		a obtener la información de ese registro y 
		lo eliminamos, por último eliminamos la regla 
		que generó el registro

En este caso, queremos eliminar las reglas  que se
realizan en este módulo ya que bloquean el tráfico
para direcciones de la red local y así solo prohibimos
esa dirección durante un intervalo de tiempo, no para 
siempre
--------------------------------------------------------*/


void detecta_Registro_caducado(char *nombre_fichero){

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura para almacenar la información de la regla
	struct ESTRUCTURA_REGLA informacion_regla;
	//Variable para recorrer la estructura
	int cont_aux_frases_fichero;
	//Variable auxiliar para almacenar el instante de tiempo actual
	time_t tiempo_actual = time(NULL);
	//Array para almacenar la fecha
	char t_actual[TAM_FECHA];
	//Estructura para almacenar hora en el momento deseado
	struct tm *tm;


	//Inicializamos
	cont_aux_frases_fichero = INICIO;

	//Leemos el fichero
	contenido_del_fichero = lee_fichero(nombre_fichero);

	//Obtenemos el instante en el que lo hemos detectado
	tm = localtime(&tiempo_actual);
			
	//Nos interesa obtener la hora y los minutos para realizar la comparación
	strftime(t_actual, sizeof(t_actual), "%H%M", tm);
	
	//Recorremos la estructura
	for (cont_aux_frases_fichero=0;
		cont_aux_frases_fichero < contenido_del_fichero.num_frases_fichero;
		cont_aux_frases_fichero++){

		//Comprobamos si ha pasado una hora desde la hora de creación de la regla
		if ((atoi(t_actual) - atoi(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][HORA]) >= PASA_UNA_HORA) ||
			(atoi(t_actual) - atoi(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][HORA]) < INICIO)) {

			//Rellenamos la estructura con la información que hemos recogido del fichero donde registramos las reglas
			informacion_regla.accion[INICIO] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][POS_ACCION+1]);
			informacion_regla.protocolo[INICIO] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][POS_PROTOCOLO+1]);
			informacion_regla.dir_IP[INICIO] = strdup(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][POS_DIR_IP_ORIG+1]);

			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][POS_BANDERA_IP],"true")==IGUAL) {
				informacion_regla.dir_en_origen[INICIO] = true;
			}
			else {
				informacion_regla.dir_en_origen[INICIO] = false;
			}

			if (strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][POS_BANDERA_PUERTO],"true")==IGUAL){
				informacion_regla.dir_Con_Puerto[INICIO] = true;
			}
			else {
				informacion_regla.dir_Con_Puerto[INICIO] = false;
			}
			//Eliminamos el registro
			elimina_Registro(nombre_fichero, cont_aux_frases_fichero);
			//Eliminamos la regla que creó ese registro
			elimina_Regla(FICHERO_REGLAS_SNORT,informacion_regla, INICIO);
			//Recargamos la configuración de SNORT para que elimine la regla
			recarga_Snort();
		}

	}

}


/*-----------------------------------------------------
	Función que se encarga de eliminar una línea del
	fichero de registro de reglas

	Recibe: El nombre del fichero de registro y la
	línea que se desea eliminar

	Devuelve: Nada

	Características:
		Lee el fichero y lo almacena en un array,
		posteriormente recorre ese array saltandose
		la línea que se desea elminar y escribe cada
		línea
-----------------------------------------------------*/

void elimina_Registro(char *nombre_fichero, int numero_linea){

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Fichero donde escribiremos todos los registros menos el que se desea borrar
	FILE *fichero_borrar;
	//Variable auxiliar para almacenar el registro que vamos a sobreescribir
	char *registro_aux = (char *)malloc(sizeof(char) *NUM_CARACTERES_REGLAS);
	//Variables para recorrer la tabla con el contenido del fichero
	int cont_aux_frases_fichero;
	int cont_aux_palabras_fichero;
	
	//Inicializamos las variables
	cont_aux_frases_fichero = INICIO;
	cont_aux_palabras_fichero = INICIO;


	//Leemos el contenido del fichero
	contenido_del_fichero = lee_fichero(nombre_fichero);

	//Abrimos el fichero vacío
	fichero_borrar = fopen(nombre_fichero, "w");

	//Recorremos el array que rellenamos cuando lo leimos por primera vez
	for (cont_aux_frases_fichero=0;
		cont_aux_frases_fichero < contenido_del_fichero.num_frases_fichero;
		cont_aux_frases_fichero++){
			
		//Comprobamos que la línea que vamos a escribir no es la que queremos eliminar
		if (cont_aux_frases_fichero != numero_linea) {

			//Recorremos las palabras de esa frase
			for(cont_aux_palabras_fichero =0;
				cont_aux_palabras_fichero <= contenido_del_fichero.palabras_por_frase[cont_aux_frases_fichero];
				cont_aux_palabras_fichero++) {
				
				//Si es la pirmera palabra
				if (cont_aux_palabras_fichero == INICIO){

					//Copiamos esa palabra en nuestro string auxiliar
					strcpy(registro_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
							
				}
				
				//Si es una palabra distinta a la primera
				else {
					//Concatenamos el contenido al string auxiliar
					strcat(registro_aux, " ");
					strcat(registro_aux, contenido_del_fichero.contenido_leido_del_fichero[cont_aux_frases_fichero][cont_aux_palabras_fichero]);
					
				}
			}

			//Añadimos un salto de línea
			strcat(registro_aux, "\n");
			//Escribimos el contenido en el fichero
			fputs(registro_aux, fichero_borrar);
		}
	}

	//Cerramos el fichero
	fclose(fichero_borrar);

	//Liberamos memoria
	free(registro_aux);
}