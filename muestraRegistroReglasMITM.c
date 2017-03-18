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

				printf("<form method=\"get\" action=\"../cgi/muestraClientesRegistrados.html\" style=\"display:inline\">\n");

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

			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraRegistroReglasMITM.out\" class=\"reglas\">\n");
					
					contenido_del_fichero = lee_fichero("../cgi/registro_reglas_MITM");
					
					if (contenido_del_fichero.num_frases_fichero == INICIO)
						printf("<p class=\"info_regla\">No hay registro<br></p>\n");

					else {

						printf("<table>\n");
						
							printf("<tr>\n");
								printf("<th>Hora</th>\n");
								printf("<th>Accion</th>\n");
								printf("<th>Protocolo</th>\n");
								printf("<th>IP</th>\n");
								printf("<th>Dir. en Origen</th>\n");
								printf("<th>Dir. con Puerto</th>\n");
							printf("</tr>\n");
							
							for (cont_aux_lineas=0;
								cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
								cont_aux_lineas++){
								
								printf("<tr>\n");	
								
									for (cont_aux_palabras_por_linea=0;
										cont_aux_palabras_por_linea<=contenido_del_fichero.palabras_por_frase[cont_aux_lineas];
										cont_aux_palabras_por_linea++) {

										printf("<td>%s</td>", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][cont_aux_palabras_por_linea]);
									}

								printf("</tr>\n");
							}	
							
						printf("</table>\n");
					}	
					printf("<button type=\"submit\">\n");
						printf("Actualizar registro\n");
					printf("</button>\n");


			printf("</form>\n");
		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}