#include <stdio.h>
#include <stdlib.h>

int main () {
	
	//CAMBIAR POR LA ORDEN PARA IPTABLES
	//system("echo \"Hola\" > /home/dit/web/cgi/reglas_iptables");

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

			printf("<p>\n");

				printf("<form method=\"get\" action=\"http://localhost/dit/cgi-bin/muestraReglasIptables.out\" style=\"display:inline\">\n");

					printf("<button type=\"submit\" class=\"button1\">\n");
						printf("Actualizar reglas\n");
					printf("</button>\n");	

				printf("</form>\n");

			printf("</p>\n");	
	
			printf("<div>\n");

				printf("<p>\n");

					printf("<iframe src=\"../cgi/reglas_iptables\" \"  >\n");
					printf("</iframe>\n");
				
				printf("</p>\n");
			
			printf("</div>\n");

		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}