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


int main (){
	
	FILE *fichero;
	fichero = fopen("alert", "r");
	struct CONTENIDO_FICHERO contenido_del_fichero;
	contenido_del_fichero = lee_fichero(fichero);
	fclose(fichero);
	
	for (int i=0; i< contenido_del_fichero.num_frases_fichero; i++){
		for (int j=0; j<= contenido_del_fichero.palabras_por_frase[i]; j++){
			fprintf(stdout,"%s ", contenido_del_fichero.contenido_leido_del_fichero[i][j]);
		}
		printf("\n");
	}
	return 0;
} 	

