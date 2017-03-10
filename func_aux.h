/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes
necesarias para que estas funcionen
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#ifndef func_aux
#define func_aux


#define IGUAL 0
/*-------------------------------------------------------
	Constante para definir el número máximo de línea que
	contiene el fichero, sin contar con las líneas
	en blanco 
-------------------------------------------------------*/
#define NUM_FRASES 500
//Constante para definir el número máximo de palabras en una frae
#define NUM_PALABRAS_POR_FRASE 50
//Constante para definir el número máximo de caracteres para una palabra
#define NUM_CARACTERES_PALABRA 100
#define INICIO 0
#define TABULADOR '\t'
#define SALTO_DE_LINEA '\n'
#define ESPACIO ' '
#define PUNTO '.'
#define TERMINADOR '\0'
//Constante para definir el número máximo de octetos en una dirección IP
#define TAM_IP 4
//Constates para definir la posición dentro de una tabla de cada fragmento
#define FRAG_1 0
#define FRAG_2 1
#define FRAG_3 2
//Constantes para definir el valor de cada fragmento de IP de la red local
#define VALOR_FRAG_1 "10"
#define VALOR_FRAG_2 "10"
#define VALOR_FRAG_3 "10"
//El número máximo de digitos que puede tener un fragmento de IP
#define MAX_DIG_FRAGMENTO_IP 3
//Constante para definir la línea de fin de una coincidencia
#define FIN_REGLA_ALERTA 3
#define SIGUIENTE_LINEA 1
//Constante para definir la posición relativa del protocolo
#define LINEA_PROTOCOLO 2
//Constante para definir la posición de la IP origen dentro de una línea
#define POSICION_IP_ORIGEN 3
//Igual que el anterior pero relativa a la IP destino
#define POSICION_IP_DESTINO 5
//Número máximo de reglas que almacenarán las tablas
#define NUM_REGLAS 100
//Constante para definir los protocolos
#define UDP "UDP"
#define TCP "TCP"
#define ICMP "ICMP"
//Constante para definir el número máximo de caracteres para una regla
#define NUM_CARACTERES_REGLAS 500
//Constante para definir el número máximo de digitos que puede tener el SID
#define NUM_DIG_SID 10
//Constantes para definir la posición que ocupan los campos relevantes en una regla
#define POS_ACCION 0
#define POS_PROTOCOLO 1
#define POS_DIR_IP_ORIG 2
#define POS_PUERTO_ORIG 3
#define POS_DIR_IP_DEST 5
#define POS_PUERTO_DEST 6
//Constante para definir el caracter :
#define DOS_PUNTOS ':'
//Constante para definir el número máximo de digitos de un puerto
#define TAM_PUERTO 6
//Constante para definir el número máximo de digitos que puede tener la fecha
#define TAM_FECHA 100
//Constante para definir el límite a partir del cuál crearemos un nuevo fichero (en mod1_DoS.c)
#define LIMITE_LINEAS_LEIDAS NUM_FRASES-30
//Constante que devolverá la funcion busca_Regla cuando no encuentre una igual
#define NO_COINCIDE -1
//Constante de tiempo para definir los segundos que pasaran entre actividad
#define INTERVALO_LECTURA 3
//Constante para almacenar el nombre del semáforo para los ficheros de lecturas
#define SEM_LECTURA "semaforo_lectura"
//Constante para definir el nombre del semáforo que se usará para la recarga de Snort
#define SEM_SNORT "semaforo_snort"
//Constante para definir el nombre del semáforo que usará el fichero donde se almacena el sid
#define SEM_SID "semaforo_sid"
//Constante para definir el nombre del semáforo que usará el fichero de reglas de snort
#define SEM_REGLAS_SNORT "semaforo_reglas_snort"
//Constante para definir el valor inicial del semáforo
#define INICIO_SEM 1


typedef struct CONTENIDO_FICHERO{

	//Variable auxiliar para almacenar cuantas palabras tiene cada frase
		//NUM_FRASES igual que en el caso anterior
	int palabras_por_frase[NUM_FRASES];
	
	//Variable para contabilizar el número de frases del fichero
	int num_frases_fichero;
	
	//Array para almacenar el contenido del documento
		//NUM_FRASES líneas y NUM_PALABRAS_POR_FRASE palabras por líneas
	char *contenido_leido_del_fichero[NUM_FRASES][NUM_PALABRAS_POR_FRASE];

} contenido_fichero;


typedef struct ESTRUCTURA_REGLA {
	
	//Array para almacenar la dirección IP
	char *dir_IP[NUM_REGLAS];
	/*--------------------------------------
		Bandera para conocer si se la 
		dirección IP lleva asociada un puerto
		o no
			--true: La dir_IP lleva un puerto
			-- false: No lo lleva
	--------------------------------------*/
	bool dir_Con_Puerto[NUM_REGLAS];
	//Array para almacenar el número de puerto
	char *puerto[NUM_REGLAS];
	//Array de string para almacenar el protocolo
	char *protocolo[NUM_REGLAS];
	/*--------------------------------------
		Bandera para conocer si la dirección
		IP se encontraba en el campo de origen
		o en el de destino
			-- true: se encontraba en origen
			-- false: se encontraba en destino
	--------------------------------------*/
	bool dir_en_origen[NUM_REGLAS];
	//Array para almacenar la acción que queremos ejecutar
	char *accion[NUM_REGLAS];
	//Variable para almacenar el número total de coincidencias
	int numero_lineas;

}estructura_regla;



/* -------------------------------------------------------
	Función que se encarga de la lectura de los ficheros 
	Recibe: El nombre del fichero que queremos leer
	Devuelve: Una estructura del tipo CONTENIDO_FICHERO
----------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero (char *nombre_fichero);


/* -------------------------------------------------------
	Función que se encarga de determinar si una IP pertenece
	a una red local o no

	Recibe: Un string que contiene la dirección IP
	Devuelve: Un booleano indicando si pertenece a la red local
----------------------------------------------------------*/
bool comprueba_IP(char *direccion_IP);


/* -------------------------------------------------------
	Función que para un string dado comprueba si dicho
	string está en el contenido leido de un fichero
	
	Recibe: Una estructura del tipo CONTENIDO_FICHERO
	el nombre que vamos a buscar y la línea por la
	que debe empezar a buscar
	Devuelve: Estructura del tipo ESTRUCTURA_REGLA
----------------------------------------------------------*/
struct ESTRUCTURA_REGLA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia, int linea_inicio);


/* -------------------------------------------------------
	Función que se encarga de crear las reglas a partir de la 
	estructura y la acción a tomar,
	
	Recibe: El nombre del fichero donde estan  almacenadas las 
		reglas , una estructura del tipo ESTRUCTURA_REGLA
		y un string para añadir información
	
	Devuelve: Un booleano indicando si se ha escrito la 
	regla o no
----------------------------------------------------------*/
bool crea_y_escribe_regla(char *nombre_fichero_escritura, struct ESTRUCTURA_REGLA contenido_fichero_alerta,char *info_extra);




/* -------------------------------------------------------
	Función que se encarga de buscar en el fichero 
	de reglas donde se quiera insertar si existe 
	una igual para evitar introducir reglas duplicadas

	Recibe: Estructura del tipo CONTENIDO_FICHERO,
	estructura del tipo ESTRUCTURA_REGLA, y la posición 
	dentro de la estructura ESTRUCTURA_REGLA donde se 
	encuentra leyendo la info relativa a la nueva regla

	Devuelve: Un entero
		-->0: Ya hay una regla igual. Es la línea que
		ocupa en el fichero donde están las reglas
		-- -1: No la hay

----------------------------------------------------------*/
int busca_Regla(struct CONTENIDO_FICHERO contenido_del_fichero_reglas, struct ESTRUCTURA_REGLA contenido_fichero_alerta, 
	int pos_dentro_estruct_regla);


/* -------------------------------------------------------
	Función para que Snort vuelva a leer las reglas 
	nuevas que se han introducido

	Recibe: Nada
	Devuelve: Nada
----------------------------------------------------------*/
void recarga_Snort ();


/*---------------------------------------------------
	Función que se encarga de vaciar un fichero
	que está en el límite de líneas que puede
	leer la función que se encarga de ello

	Devuelve: string con el nombre del fichero creado

	Recibe: El nombre del fichero
---------------------------------------------------*/
char *vacia_fichero(char *nombre_fichero);


/*--------------------------------------------------------
	Función que se encarga de eliminar una regla
	del fichero de reglas de Snort

	Recibe: El fichero del que se quiere eliminar
	las reglas, la estructura del tipo ESTRUCTURA_REGLA
	que contiene la información de la regla que se
	desea borrar y la posición dentro de la 
	estructura

	Devuelve: Nada
------------------------------------------------------*/
void elimina_Regla(char *nombre_fichero_borrar, struct ESTRUCTURA_REGLA informacion_regla, int pos_dentro_estruct_regla);



/*----------------------------------------------------
	Función que se encarga de crear un semáforo

	Recibe: Nombre del semaforo a crear
	Devuelve: Nada
----------------------------------------------------*/
void crea_semaforo(char *nombre_semaforo);



/*---------------------------------------------
	Función que se encarga de eliminar un
	semáforo que fue creado previamente

	Recibe: El nombre del semáforo a eliminar
	Devuelve: Nada
---------------------------------------------*/
void elimina_semaforo(char *nombre_semaforo);


/*----------------------------------------
	Función que implementa el primer 
	módulo

		Recibe: Nada
		Devuelve: Nada
----------------------------------------*/

void mod1();
#endif
