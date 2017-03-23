/*-----------------------------------------------
	Fecha: 17-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que se encarga de mostrar
	las reglas definidas en Snort y mediante
	un botón selecciona cual queremos eliminar
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
	//Variable para indicar el ID de cada regla
	int id_regla;

	//Inicializamos
	cont_aux_lineas = INICIO;
	cont_aux_palabras_por_linea = INICIO;	
	id_regla = INICIO;

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
			//CREAMOS LA BOTONERA SUPERIOR
			printf("<p>\n");

				/*-----------------------------------------------------
					Indicamos la dirección a la que nos dirigimos si se pulsa 
					ese botón, el método con el que se realizará la petición y
					el estilo del botón, que será en línea para que todos aparezcan
					en una única fila en la parte superior
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

				printf("<form method=\"get\" action=\"http://localhost/dit/jsp/muestraLogsWIPS.jsp\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Logs WIPS\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/borraRegla.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Borrar Reglas\n");
					printf("</button>\n");

				printf("</form>\n");

			printf("</p>\n");

			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/procesa_borrado_regla.out\" class=\"reglas\">\n");
					
					//Leemos el fichero de reglas de Snort
					contenido_del_fichero = lee_fichero("../cgi/local.rules");
					
					//Recorremos la estructura que se devuelve al leer el fichero
					for (cont_aux_lineas=0;
						cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
						cont_aux_lineas++){
						
						//Creamos un nuevo párrafo por cada regla
						printf("<p class=\"borrar_regla\">\n");
						
						//Incluimos un botón de tipo radio para poder seleccionar la regla que queremos eliminar	
						printf("<input type=\"radio\" name=\"regla\" id=\"%d\" value=\"%d\">\n", id_regla, id_regla);
						id_regla++;
						
							//Imprimimos la regla
							for (cont_aux_palabras_por_linea=0;
								cont_aux_palabras_por_linea<=contenido_del_fichero.palabras_por_frase[cont_aux_lineas];
								cont_aux_palabras_por_linea++) {

								printf("%s ", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][cont_aux_palabras_por_linea]);
							}

						printf("<br>\n");
						printf("</p>\n");
					}	
					//Si no se ha leido ninguna regla, lo indicamos
					if (contenido_del_fichero.num_frases_fichero == INICIO)
						printf("<p class=\"info_regla\">No hay reglas<br></p>\n");
					
					//Botón para enviar la petición de la regla que deseamos elimianr
					printf("<button type=\"submit\" id=\"button2\">\n");
						printf("Borrar regla\n");
					printf("</button>\n");


			printf("</form>\n");
		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}