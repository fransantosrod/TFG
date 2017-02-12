/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes
necesarias para que estas funcionen
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

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


typedef struct COINCIDENCIA_REGLA {
	
	//Array para almacenar la dirección IP
	char *dir_IP[NUM_REGLAS];
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
	//Variable para almacenar el número total de coincidencias
	int numero_coincidencias;

}coincidencia_regla;


/* -------------------------------------------------------
	Función que se encarga de la lectura de los ficheros 
	Recibe: Un fichero abierto para su lectura
	Devuelve: Una estructura del tipo CONTENIDO_FICHERO
----------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero (FILE *fichero_lectura);


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
	Devuelve: Estructura del tipo COINCIDENCIA_REGLA
----------------------------------------------------------*/
struct COINCIDENCIA_REGLA comprueba_Coincidencia_Fichero_Leido(struct CONTENIDO_FICHERO contenido_del_fichero, char * nombre_Coincidencia);

#endif