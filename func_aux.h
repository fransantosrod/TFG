/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 12-02-2017
Descripción: Fichero que contiene las declaraciones
de las funciones auxiliares y constantes
necesarias para que estas funcionen
-------------------------------------------*/
#include <stdio.h>


#ifndef func_aux
#define func_aux

#define NUM_FRASES 500
#define NUM_PALABRAS_POR_FRASE 50
#define NUM_CARACTERES_PALABRA 100
#define INICIO 0
#define SALTO_DE_LINEA '\n'
#define ESPACIO ' '

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


/* -------------------------------------------------------
	Función que se encarga de la lectura de los ficheros 
	Recibe una fichero abierto para su lectura
	Devuelve: Una estructura del tipo CONTENIDO_FICHERO
----------------------------------------------------------*/
struct CONTENIDO_FICHERO lee_fichero (FILE *fichero_lectura);



#endif