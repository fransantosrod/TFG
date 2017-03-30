/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 20-02-2017
Descripción: Fichero que contiene las 
implementaciones del segundo módulo centrado
en los ataques MITM 
-------------------------------------------*/
#include "func_aux_mod2.h"

/*-------------------------------------------
	Función que se encarga de implementar
	las funcionalidades del módulo 2

	Recibe: Nada

	Devuelve: Nada
-------------------------------------------*/

void *mod2 () {

	//Estructura que almacenará los datos relativos al fichero
	struct CONTENIDO_FICHERO contenido_del_fichero;
	//Estructura que almacenará los datos relativos a las reglas que queremos crear
	struct ESTRUCTURA_REGLA informacion_regla;
	//Variable donde almacenamos el fichero que queremos leer
	char *nombre_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Variable auxiliar para conocer el número de línea que leímos anteriormente
	int num_lineas_anterior;
	//Variable auxiliar para almacenar el nombre del fichero que se crea
	char *nuevo_fichero = (char *)malloc(sizeof(char)*NUM_CARACTERES_PALABRA);
	//Bandera para indicar si se han creado reglas
	bool crear_regla;

	//Inicalizamos las variables
	strcpy(nombre_fichero, FICHERO_ARPWATCH);
	num_lineas_anterior = INICIO;
	crear_regla = false;

	while(true){

		//Leemos el fichero
		contenido_del_fichero = lee_fichero(nombre_fichero);

		//Miramos a ver si para algún registro ha pasado el tiempo
		/*---------------------------------------------------
			Esto se hace ya que para los ataques MITM, las
			direcciones que vamos a bloquear son IPs 
			pertenecientes a la red interna, luego,este 
			bloqueo sólo será efectivo durante un periodo
			"largo" de tiempo para impedir el ataque y 
			permitir que esa dirección esté disponible 
			en un futuro
		---------------------------------------------------*/
		detecta_Registro_caducado(FICHERO_REGISTRO_REGLAS_MITM);
				
		//Comprobamos si el número de líneas leídas está cerca del máximo (NUM_FRASES)
		if ( contenido_del_fichero.num_frases_fichero < LIMITE_LINEAS_LEIDAS){
				
			//Comprobamos si hemos leído alguna línea nueva
			if (contenido_del_fichero.num_frases_fichero > num_lineas_anterior){
					
				//Buscamos si hemos detectado un cambio en la MAC
				informacion_regla=busca_CAMBIO_EN_MAC(contenido_del_fichero, num_lineas_anterior);
				
				//Registramos la regla que vamos a crear
				registra_Regla(informacion_regla);

				//Comprobamos si tenemos que crear alguna regla
				if (informacion_regla.numero_lineas > INICIO){

					//Creamos la regla
					crear_regla = crea_y_escribe_regla(FICHERO_REGLAS_SNORT, informacion_regla, "MITM");
					
					
					//Comprobamos si la hemos creado correctamente
					if (crear_regla == true) {
							
						//Cambiamos el valor de la bandera para que solo reiniciemos cuando se crea una regla nueva
						crear_regla = false;
						
						//Reiniciamos Snort
						recarga_Snort();
					}
				}
			}
		}
			
		//En el caso en el que estemos cerca del límite
		else {
				
			//Llamamos a la función deseada para vaicar el fichero
			nuevo_fichero = vacia_fichero(nombre_fichero);
				
			/*--------------------------------------------------------
				Para asegurarnos que no perdemos información
				en ningún momento, volvemos a leer el fichero
				al que le hemos cambiado el nombre siempre y
				cuando no supere el número máximo (NUM_FRASES)
				
				Esta comprobación la hacemos por si el fichero 
				se encontraba en el rango entre LIMITE y NUM_FRASES
				y hay información que se puede perder
			--------------------------------------------------------*/
			//Leemos el fichero
			contenido_del_fichero = lee_fichero(nuevo_fichero);
			
			//Comprobamos que ese fichero no ha superado el límite
			if (contenido_del_fichero.num_frases_fichero < NUM_FRASES){
					
				//Buscamos si se ha producido algún cambio en las direcciones MAC
				informacion_regla = busca_CAMBIO_EN_MAC(contenido_del_fichero, num_lineas_anterior);
				
				//Comprobamos si hemos detectado la necesidad de crear alguna regla
				if (informacion_regla.numero_lineas > INICIO){
					
					//Registramos la regla que vamos a crear
					registra_Regla(informacion_regla);		
					
					//Creamos la regla
					crear_regla = crea_y_escribe_regla(FICHERO_REGLAS_SNORT, informacion_regla, "MITM");	
					
					//Comprobamos que la hemos creado correctamente
					if (crear_regla == true){

						//Cambiamos el valor de la bandera
						crear_regla = false;

						//Recargamos Snort
						recarga_Snort();
					}
				}
					
			}

			//Reiniciamos el número de líneas para la próxima iteración
			contenido_del_fichero.num_frases_fichero = INICIO;
		}
		
		//Almacenamos el número de líneas leídas
		num_lineas_anterior = contenido_del_fichero.num_frases_fichero;
		
		//Esperamos un intervalo de tiempo para la próxima lectura
		sleep(INTERVALO_LECTURA);
	}

	//Liberamos memoria 
	free(nombre_fichero);
	free(nuevo_fichero);

	pthread_exit(NULL);

}