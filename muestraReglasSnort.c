/*-----------------------------------------------
	Fecha: 18-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que muestra las reglas
	definidas en Snort
----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "/home/dit/Desktop/TFG/func_aux.h"

int main () {
	
	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Variables auxiliares para recorrer la estructura
	int cont_aux_lineas;
	int cont_aux_palabras_por_linea;
	
	//Inicializamos
	cont_aux_lineas = INICIO;
	cont_aux_palabras_por_linea = INICIO;	
	
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
			printf("<title>Gestion WIPS </title>\n");
		printf("</head>\n");

		printf("<body>\n");
			
			//CREAMOS LA BOTONERA SUPERIOR
			printf("<p>\n");

				/*-----------------------------------------------------
					Indicamos la dirección a la que nos dirigimos si se pulsa 
					ese botón, el método con el que se realizará la petición y
					el estilo del botón, que será en línea para que todos aparezcan
					en la parte superior
				-----------------------------------------------------*/
				printf("<form method=\"get\" action=\"../cgi/wips.html\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Inicio\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasSnort.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Reglas Snort\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasIptables.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Reglas Iptables\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/jsp/muestraClientesRegistrados.jsp\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Clientes Registrados\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraRegistroReglasMITM.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Registro Reglas MITM\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/borraRegla.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Borrar Reglas\n");
					printf("</button>\n");

				printf("</form>\n");

			printf("</p>\n");

			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasSnort.out\" class=\"reglas\">\n");
					
					//Leemos el contenido del fichero		
					contenido_del_fichero = lee_fichero("../cgi/local.rules");
					
					//Creamos una lista no ordenada
					printf("<ul>\n");

					//Recorremos los elementos leidos del ficheros
					for (cont_aux_lineas=0;
						cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
						cont_aux_lineas++){
						
							//Insertamos cada elementos en un punto nuevo
							printf("<li>\n");
							for (cont_aux_palabras_por_linea=0;
								cont_aux_palabras_por_linea<=contenido_del_fichero.palabras_por_frase[cont_aux_lineas];
								cont_aux_palabras_por_linea++) {

								printf("%s ", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][cont_aux_palabras_por_linea]);
							}
							printf("</li>\n");

						printf("<br>\n");
					}	
					
					printf("</ul>\n");
					
					//En el caso que no hayamos leido ninguna regla, lo indicamos
					if (contenido_del_fichero.num_frases_fichero == INICIO)
						printf("<p class=\"info_regla\">No hay reglas<br></p>\n");

					//Botón que permite actualizar las reglas
					printf("<button type=\"submit\" id=\"button2\">\n");
						printf("Actualizar reglas\n");
					printf("</button>\n");


			printf("</form>\n");
		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}