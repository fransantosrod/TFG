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
			printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"../cgi/hoja_estilo.css\">\n");
			printf("<title>Gestion WIPS </title>\n");
		printf("</head>\n");

		printf("<body>\n");
	
			printf("<p>\n");

				printf("<form method=\"get\" action=\"../cgi/prueba.html\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Inicio\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"../cgi/muestrareglas.html\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Muestra Reglas Snort\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasIptables.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Muestra Reglas Iptables\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"../cgi/muestraClientesRegistrados.html\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Muestra Clientes Registrados\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"../cgi/muestraRegistroReglasMITM.html\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Muestra Registro Reglas MITM\n");
					printf("</button>\n");

				printf("</form>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/borraRegla.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\">\n");
						printf("Borrar Reglas\n");
					printf("</button>\n");

				printf("</form>\n");

			printf("</p>\n");

			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/procesa_borrado_regla.out\" class=\"form_borrar_reglas\">\n");
					
					contenido_del_fichero = lee_fichero("../cgi/local.rules");
					

					for (cont_aux_lineas=0;
						cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
						cont_aux_lineas++){
						
						printf("<p>\n");	
						printf("<input type=\"radio\" name=\"regla\" id=\"%d\" value=\"%d\">\n", id_regla, id_regla);
						id_regla++;

							for (cont_aux_palabras_por_linea=0;
								cont_aux_palabras_por_linea<=contenido_del_fichero.palabras_por_frase[cont_aux_lineas];
								cont_aux_palabras_por_linea++) {

								printf("%s ", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][cont_aux_palabras_por_linea]);
							}

						printf("<br>\n");
						printf("</p>\n");
					}	
					

					printf("<button type=\"submit\" id=\"button2\">\n");
						printf("Borrar regla\n");
					printf("</button>\n");


			printf("</form>\n");
		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}