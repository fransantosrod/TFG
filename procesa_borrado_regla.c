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
			printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"../cgi/hoja_estilo.css\">\n");
			printf("<title>Gestion WIPS </title>\n");
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
				informacion_regla.numero_lineas = INICIO;
				
				elimina_Regla("../cgi/local.rules",informacion_regla, atoi(regla));

				printf("<p class=\"info_regla\">Regla %d eliminada correctamente\n</p>", atoi(regla));
			}	
			
			else {
				printf("<p class=\"info_regla\">No se ha podido eliminar la regla</p>\n");
			}
			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/borraRegla.out\">\n");
				
				printf("<button type=\"submit\">\n");
					printf("Volver\n");
				printf("</button>\n");

			printf("</form>\n");

		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}