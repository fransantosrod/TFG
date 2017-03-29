/*-----------------------------------------------
	Fecha: 17-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que recibe la petición
	de la regla a eliminar y la borra
----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/pi/TFG/func_aux.h"
#include "/home/pi/TFG/func_aux_mod2.h"


void elimina_Registro_En_Web(int num_regla);


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
				
				/*-------------------------------------
					En el caso que queramos eliminar
					una regla creada por un ataque
					MITM, esta llevará asociada una 
					límea de registro en otro fichero
					que debemos borrar también
				-------------------------------------*/
				elimina_Registro_En_Web(atoi(regla));

				//Eliminamos la regla
				/*---------------------------------------------------
					Hemos creado un enlace simbólico desde 
					/etc/snort/rules/local.rules a /$DIR_WEB/cgi/
					de esta forma accedemos directamente
				---------------------------------------------------*/
				elimina_Regla("../cgi/local.rules",informacion_regla, atoi(regla));

				//Recargamos Snort para que los cambios tengan efecto
				recarga_Snort();

				
				//Informamos la regla que se ha eliminado
				printf("<p class=\"info_regla\">Regla %d eliminada correctamente\n</p>", atoi(regla)+1);
			}	
			
			//En caso de que no se reciban datos
			else {

				//Indicamos que no se ha borrado ninguna regla
				printf("<p class=\"info_regla\">No se ha podido eliminar la regla</p>\n");
			}
			

			printf("<form method=\"get\" action=\"http://10.10.10.2/wips/cgi-bin/borraRegla.out\">\n");
				
				//Botón para volver a la página de  reglas
				printf("<button type=\"submit\">\n");
					printf("Volver\n");
				printf("</button>\n");

			printf("</form>\n");

		printf("</body>\n");
	
	printf("</html>\n");

return 0;
}



/*--------------------------------------------
	Función que para una regla perteneciente
	al fichero local.rules, busca si es 
	un registro de una regla MITM y borra
	este en caso afirmativo

	Recibe: Un entero indicando la posición
	de la regla dentro de la estructura

	Devuelve: Nada
--------------------------------------------*/
void elimina_Registro_En_Web(int num_regla) {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura donde almacenaremos la información de la regla
	struct ESTRUCTURA_REGLA informacion_regla;

	//Variables auxiliares para recorrer la estructura
	int cont_aux_lineas;
	//Variable para almacenar la línea de coincidencia de la regla
	int linea_coincidencia;

	//Inicializamos
	linea_coincidencia = NO_COINCIDE;

	//Leemos el fichero de reglas
	contenido_del_fichero = lee_fichero("../cgi/local.rules");

	//Recorremos la estructura que nos devuelve la función
	for (cont_aux_lineas=0;
			cont_aux_lineas< contenido_del_fichero.num_frases_fichero;
			cont_aux_lineas++) {

		//Si estamos en el número de regla que deseamos eliminar del registro
		if (cont_aux_lineas == num_regla) {

			//Almacenamos la información de la regla para encontrar el registro correspondiente
			informacion_regla.accion[INICIO] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][POS_ACCION];

			informacion_regla.protocolo[INICIO] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][POS_PROTOCOLO];
			
			
			if ( strcmp(contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][POS_DIR_IP_ORIG],"$HOME_NET") != IGUAL ){

				informacion_regla.dir_IP[INICIO] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][POS_DIR_IP_ORIG];
				informacion_regla.dir_en_origen[INICIO] = true;
			
			}

			else {

				informacion_regla.dir_IP[INICIO] = contenido_del_fichero.contenido_leido_del_fichero[cont_aux_lineas][POS_DIR_IP_DEST];
				informacion_regla.dir_en_origen[INICIO] = false;	
				
			}
			//No se almacena la información del puerto ya que sabemos que por defecto es "false" porque no llevan puertos asociados

		}


	}

	//Buscamos si la regla estaba en el fichero de registro
	linea_coincidencia = busca_Registro("../cgi/registro_reglas_MITM",informacion_regla, INICIO);
	
	//Si ha encontrado un registro que coincida con la regla borrada
	if (linea_coincidencia != NO_COINCIDE) {

		//Eliminamos el registro
		elimina_Registro("../cgi/registro_reglas_MITM", linea_coincidencia);
		//Lo indicamos por la interfaz
		printf("<p class=\"info_regla\">Registro de regla eliminado correctamente<p>\n");
	}


}