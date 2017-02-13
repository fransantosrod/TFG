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
	
	//Variable para el fichero que se desea leer
	FILE *fichero_lectura;
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las coincidencias
	struct CONTENIDO_ALERTA contenido_fichero_alerta;

	//Inicializamos las variables
	fichero_lectura = fopen("alert_prueba", "r");
	

	if (fichero_lectura != NULL){

		contenido_del_fichero = lee_fichero(fichero_lectura);
		fclose(fichero_lectura);
	
		contenido_fichero_alerta = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
		
		crea_y_escribe_regla("local.rules_prueba", contenido_fichero_alerta, "drop");
			
	}
	else
		fprintf(stderr, "Error al abrir el fichero\n");
	return 0;

} 	