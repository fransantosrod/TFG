/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes
necesarias para que estas funcionen
-------------------------------------------*/
#include "constantes_y_librerias.h"

#ifndef func_aux
#define func_aux

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

/*---------------------------------------------
	Función que se encarga de registrar en
	el fichero de logs del WIPS las incidencias
	que aparecen
	
	Recibe: La causa del log
	Devuelve: Nada
---------------------------------------------*/
void registra_log(char *log);
#endif
