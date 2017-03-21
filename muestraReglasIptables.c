/*-----------------------------------------------
	Fecha: 16-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que se encarga de 
	mostrar las reglas definidas en IPTABLES
----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

int main () {
	
	//CAMBIAR POR LA ORDEN PARA IPTABLES
	//system("sudo iptables -L -n > /home/dit/web/cgi/reglas_iptables");

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

			printf("<p>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasIptables.out\" style=\"display:inline\">\n");

					//Botón para actualizar el registro de reglas de iptables
					printf("<button type=\"submit\" class=\"button1\">\n");
						printf("Actualizar reglas\n");
					printf("</button>\n");	

				printf("</form>\n");

			printf("</p>\n");	
	
			printf("<div>\n");

				printf("<p>\n");

					/*----------------------------------------------------------
						Mediante iframe mostramos el contenido del fichero
						donde hemos alamcenado la salida del comando 
						"iptables -L -n"
					----------------------------------------------------------*/
					printf("<iframe src=\"../cgi/reglas_iptables\" \"  >\n");
					printf("</iframe>\n");
				
				printf("</p>\n");
			
			printf("</div>\n");

		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}