#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_FRASES 500
#define NUM_PALABRAS_POR_FRASE 50
#define NUM_CARACTERES_PALABRA 100
#define INICIO 0
#define SALTO_DE_LINEA '\n'
#define ESPACIO ' '
typedef struct CONTENIDO_FICHERO{

	//Variable auxiliar para almacenar cuantas palabras tiene cada frase
		//NUM_FRASES igual que en el caso anterior
	int palabras_por_frase[NUM_FRASES];
	//Variable para contabilizar el número de frases del fichero
	int num_frases_fichero;
	//Array para almacenar el contenido del documento
		//NUM_FRASES líneas y NUM_PALABRAS_POR_FRASE palabras por líneas
	char *contenido_leido_del_fichero[NUM_FRASES][NUM_PALABRAS_POR_FRASE];

} contenido_fichero;

struct CONTENIDO_FICHERO lee_fichero (FILE *fichero_lectura);

int main (){
	
	FILE *fichero;
	fichero = fopen("alert", "r");
	struct CONTENIDO_FICHERO contenido_del_fichero;
	contenido_del_fichero = lee_fichero(fichero);
	fclose(fichero);
	
	for (int i=0; i< contenido_del_fichero.num_frases_fichero; i++){
		for (int j=0; j<= contenido_del_fichero.palabras_por_frase[i]; j++){
			printf("%s ", contenido_del_fichero.contenido_leido_del_fichero[i][j]);
		}
		printf("\n");
	}
	return 0;
} 	

/* -------------------------------------------------------
	Función que 
	Recibe una fichero abierto para su lectura
	Devuelve: Una estructura con
		-- Número de frases leidas
		-- Número de palabras en cada frase
		-- Array doble que almacena cada palabra de cada frase

	Características:
		El fichero no debe contener más de NUM_FRASES
		formada por no más de NUM_PALABRAS_POR_FRASE
		cada palabra de las frase no debe tener más de
		NUM_CARACTERES_PALABRA, estos parámetros están definidos
		en la estructura
--------------------------------------------------------*/

struct CONTENIDO_FICHERO lee_fichero (FILE *fichero_lectura){

	//Variable para leer el fichero caracter a carater
	char c;
	//Variable para almacenar el valor del caracter anterior
	char c_ant;
	
	//Variable para contabilizar el número de frases del fichero
	int frase;
	//Variable para contabilizar en que palabra estamos
	int palabra;
	//Variable para contabilizar el número de letras
	int letra;
	//Bandera para bloquear el aumento de frases si estamos leyendo un salto de parrafo
	bool flag;
	
	//Array auxiliar para almacenar cada palabra de la frase
		//NUM_CARACTERES_PALABRA caracteres por palabras
	char *palabras_frase = (char *)malloc (sizeof(char)*NUM_CARACTERES_PALABRA);
	
	//Estructura que será devuelta
	struct CONTENIDO_FICHERO contenido_del_fichero;

	//Inicialización de variables
	frase = INICIO;
	c_ant = ESPACIO;
	palabra = INICIO;
	letra = INICIO;
	flag = false;


	if (fichero_lectura != NULL){
		//Comenzamos a leer el fichero
		while ((c=fgetc(fichero_lectura))!=EOF){
		
			if (c!= ESPACIO && c!=SALTO_DE_LINEA){
				//Almacenamos el caracter leido
				palabras_frase[letra] = c;
				//Aumentamos el número de letras leidas
				letra++;
			}
		
			//Comprobamos si el caracter leido anteriormente y el actual son un salto de linea
			else if (c_ant == SALTO_DE_LINEA && c == SALTO_DE_LINEA){

				//Si lo es, estamos en un nuevo parrafo
				flag=true;
				palabra=INICIO;
			
			}
		
			//Comprobamos si se ha leido alguna letra,si esta esta es un salto de línea y no estamos en el salto de parrafo 
			else if (c == SALTO_DE_LINEA && letra>0 && flag!= true){
			
				//Almacenamos la palabra de la frase deseada en nuestra tabla
				palabras_frase[letra++] = '\0';
				contenido_del_fichero.contenido_leido_del_fichero[frase][palabra] = strdup(palabras_frase); 
				
				//Volvemos a iniciar el número de letras ya que estamos en una nueva frase y palabra
				letra = INICIO;
				//Almacenamos el número de palabras que tenía la frase que hemos terminado de leer
				contenido_del_fichero.palabras_por_frase[frase] = palabra;
				
				palabra=INICIO;	
			
				//En ese caso aumentamos el número de frases leidas
				frase++;
			
			}
		
			//Comprobamos si el caracter es un espacio y si se ha leido alguno 
			else if (c == ESPACIO && letra >0){
			
				//Almacenamos la palabra de la frase deseada en nuestra estructura
				palabras_frase[letra++] = '\0';
				contenido_del_fichero.contenido_leido_del_fichero[frase][palabra] = strdup(palabras_frase); 
				
				//Aumentamos el número de palabras leidas
				palabra++;
				//Volvemos a iniciar el número de letras leidas ya que estamos en una nueva palabra
				letra=INICIO;
			}

			c_ant=c;;
			flag=false;
		}
	}

	else 
		printf("No se puede abrir el fichero para su lectura\n");
	
	//Rellenamos la estructura para poder usarla en otro punto del codigo
	contenido_del_fichero.num_frases_fichero = frase;

	//Liberamos la memoria
	free(palabras_frase);
	
	return contenido_del_fichero;
}