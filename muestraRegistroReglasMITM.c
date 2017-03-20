/*-----------------------------------------------
	Fecha: 17-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que se encarga de mostrar
	las reglas creadas para los ataques MITM
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

			
			printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraRegistroReglasMITM.out\" class=\"reglas\">\n");
					
					//Leemos el fichero de las reglas creadas para los ataques MITM
					contenido_del_fichero = lee_fichero("../cgi/registro_reglas_MITM");
					
					//Si no hemos leido ninguna línea, indicamos que no hay reglas
					if (contenido_del_fichero.num_frases_fichero == INICIO)
						printf("<p class=\"info_regla\">No hay registro<br></p>\n");


					else {
						
						//Creamos una tabla para mostrar los registros 
						printf("<table>\n");

							//Indicamos los valores de cabecera
							printf("<tr>\n");
								printf("<th>Hora</th>\n");
								printf("<th>Accion</th>\n");
								printf("<th>Protocolo</th>\n");
								printf("<th>IP</th>\n");
								printf("<th>Dir. en Origen</th>\n");
								printf("<th>Dir. con Puerto</th>\n");
							printf("</tr>\n");
							
							//Recorremos la estructura tras haber leido el fichero
							for (cont_aux_lineas=0;
								cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
								cont_aux_lineas++){
								
								//Para cada línea, creamos una nueva fila
								printf("<tr>\n");	

									//Dentro de cada fila metemos cada palabra en una celda distinta
									for (cont_aux_palabras_por_linea=0;
										cont_aux_palabras_por_linea<=contenido_del_fichero.palabras_por_frase[cont_aux_lineas];
										cont_aux_palabras_por_linea++) {

										printf("<td>%s</td>", contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][cont_aux_palabras_por_linea]);
									}

								printf("</tr>\n");
							}	
							
						printf("</table>\n");
					}

					//Botón para acutalizar el registro de reglas	
					printf("<button type=\"submit\">\n");
						printf("Actualizar registro\n");
					printf("</button>\n");


			printf("</form>\n");
		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}