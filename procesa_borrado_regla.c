/*-----------------------------------------------
	Fecha: 17-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que recibe la petición
	de la regla a eliminar y la borra
----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/dit/Desktop/TFG/func_aux.h"

//Dado que el máximo número de reglas es 500
#define NUM_DIGITOS 3

int main () {

	//Estructura auxiliar para almacenar la info de la regla
	struct ESTRUCTURA_REGLA informacion_regla;
	//Variable para almacenar los datos recibidos
	char * datos;
	//Variable para almacenar el número de regla a eliminar
	char regla[NUM_DIGITOS];
	//Variable auxiliar para recorrer los caracteres
	int caracter;
	
	//Inicializamos
	caracter = INICIO;
	
	printf("Content-Type:text/html\n\n");
	printf("<!DOCTYPE html>\n");
	
	printf("<html>\n");
	
		printf("<head>\n");
			
			/*------------------------------------------
				Añadimos el logo en la cabecera
			------------------------------------------*/
			printf("<link rel=\"shortcut icon\" type=\"image/png\" href=\"../cgi/logo.png\"/>\n");
			
			/*-----------------------------------------------------
				Definimos en la cabecera el título de la página 
				y la hoja de estilos que se aplicará
			-----------------------------------------------------*/
			printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"../cgi/hoja_estilo.css\">\n");
			printf("<title>Gesti&oacuten WIPS </title>\n");
		
		printf("</head>\n");

		printf("<body>\n");
	
			//Recibimos los datos
			datos = getenv("QUERY_STRING");

			
			//Buscamos el caracter '=' ya que tras él vendrá el valor
			datos = strchr(datos, '=');
			//Comprobamos que hemos recibido datos
			if (datos != NULL){
				
				//Recorremos los caracteres
				for (caracter=1;caracter<(int)strlen(datos);caracter++){
					
					//Eliminamos el caracter '='
					regla[caracter-1] = datos[caracter];
				}
				//Añadimos el terminador
				regla[caracter] = SALTO_DE_LINEA;				
				//Indicamos que la estructura estará vacía
				/*---------------------------------------
					De esta forma indicamos a la función
					que se encarga de elimianr la regla
					que el tercer parámetro es la línea
					en la que se encuentra la regla 
					que se desea eliminar dentro del 
					fichero de reglas de Snort
				----------------------------------------*/
				informacion_regla.numero_lineas = INICIO;
				
				//Eliminamos la regla
				elimina_Regla("../cgi/local.rules",informacion_regla, atoi(regla));

				//Informamos la regla que se ha eliminado
				printf("<p class=\"info_regla\">Regla %d eliminada correctamente\n</p>", atoi(regla)+1);
			}	
			
			else {
				printf("<p class=\"info_regla\">No se ha podido eliminar la regla</p>\n");
			}
			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/borraRegla.out\">\n");
				//Botón para volver a la página de  reglas
				printf("<button type=\"submit\">\n");
					printf("Volver\n");
				printf("</button>\n");

			printf("</form>\n");

		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}