/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 10-02-2017
Descripción: Fichero que contiene la 
implementación del primer módulo centrado 
en los ataques DoS
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "func_aux.h"

#define NUM_CARACTERES_REGLAS 500

void crea_y_escribe_regla(FILE *fichero_escritura, struct COINCIDENCIA_REGLA coincidencias_en_reglas, char *accion);


int main (){
	
	//Variable para el fichero que se desea leer
	FILE *fichero_lectura;
	//Variable para el fichero donde deseamos escribir
	FILE *fichero_escritura;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct COINCIDENCIA_REGLA coincidencias_en_reglas;

	//Inicializamos las variables
	fichero_lectura = fopen("alert", "r");
	fichero_escritura = fopen("local.rules", "ab+");

	contenido_del_fichero = lee_fichero(fichero_lectura);
	fclose(fichero_lectura);
	

	coincidencias_en_reglas = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
	
	crea_y_escribe_regla(fichero_escritura, coincidencias_en_reglas, "drop");
	fclose(fichero_escritura);	
	return 0;

} 	

/*-----------------------------------------------
	Función que se encarga de crear las reglas
	a partir de la estructura y la acción a tomar
	buscar si existen coincidencias para esas reglas
	y las escribe

	Recibe: Un fichero, una estructura del tipo
		COINCIDENCIA_REGLA y la acción
	Devuelve: Nada

	Caracteristicas:

-----------------------------------------------*/

void crea_y_escribe_regla(FILE *fichero_escritura, struct COINCIDENCIA_REGLA coincidencias_en_reglas, char *accion){

	//Array para almacenar la regla que se va a crear
		//Tamaño NUM_REGLAS ya que no vamos a tener más de esas
	char *reglas[NUM_REGLAS];
	//Array auxiliar para ir formando la regla
		//CAMPOS_REGLAS número de palabras que contendrá como máximo una regla
	char *reglas_aux= (char *) malloc (sizeof (char)*NUM_CARACTERES_REGLAS);
	//Variable auxiliar para recorre el bucle
	int cont_aux_regla;

	//Inicializamos
	cont_aux_regla = INICIO;

	for (cont_aux_regla=0; cont_aux_regla<coincidencias_en_reglas.numero_coincidencias; cont_aux_regla++) {
		strcpy(reglas_aux,accion);
		strcat(reglas_aux, " ");
		strcat(reglas_aux, coincidencias_en_reglas.protocolo[cont_aux_regla]);
		strcat(reglas_aux, " ");
		
		if (coincidencias_en_reglas.dir_en_origen[cont_aux_regla] == true)
			strcat(reglas_aux, coincidencias_en_reglas.dir_IP[cont_aux_regla]);
		else
			strcat(reglas_aux, "any");
		strcat(reglas_aux, " ");
		
		//ESTO DEL PUERTO HAY QUE CAMBIARLO CUANDO LO INTRODUZCA EN LA ESTRUCTURA
		strcat(reglas_aux, "any");
		strcat(reglas_aux, " ");
		strcat(reglas_aux, "->");
		strcat(reglas_aux, " ");
		//HASTA AQUI

		if (coincidencias_en_reglas.dir_en_origen[cont_aux_regla] == false)
			strcat(reglas_aux, coincidencias_en_reglas.dir_IP[cont_aux_regla]);
		else
			strcat(reglas_aux, "any");
		strcat(reglas_aux, " ");
		
		//ESTO DEL PUERTO HAY QUE CAMBIARLO CUANDO LO INTRODUZCA EN LA ESTRUCTURA
		strcat(reglas_aux, "any");
		strcat(reglas_aux, " ");
		//HASTA AQUI


		strcat(reglas_aux, "(msg: ");
		strcat(reglas_aux,accion);
		strcat(reglas_aux, " ");
		strcat(reglas_aux, coincidencias_en_reglas.protocolo[cont_aux_regla]);
		strcat(reglas_aux, "; sid:");

		//HACER AUTOMÁTICO
		strcat(reglas_aux, "1000002");
		//HASTA AQUI
		strcat(reglas_aux, "; rev:1;)");
		strcat(reglas_aux, "\n");
		reglas[cont_aux_regla] = reglas_aux;
		printf("%s\n", reglas_aux);
	}

	for (cont_aux_regla=0; cont_aux_regla<coincidencias_en_reglas.numero_coincidencias; cont_aux_regla++){
		printf("%s\n", reglas[cont_aux_regla]);	
		fputs(reglas[cont_aux_regla], fichero_escritura);
	}
}


