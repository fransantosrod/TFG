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
	struct COINCIDENCIA_REGLA coincidencias_en_reglas;

	//Inicializamos las variables
	fichero_lectura = fopen("alert", "r");
	

	contenido_del_fichero = lee_fichero(fichero_lectura);
	fclose(fichero_lectura);

	coincidencias_en_reglas = comprueba_Coincidencia_Fichero_Leido(contenido_del_fichero, "DoS");
	for (int i = 0; i< coincidencias_en_reglas.numero_coincidencias; i++){

		printf("Direccion IP %s encontrada en origen: %d perteneciente al protocolo %s\n", 
			coincidencias_en_reglas.dir_IP[i], coincidencias_en_reglas.dir_en_origen[i], coincidencias_en_reglas.protocolo[i]);
	}
	
	return 0;

} 	



