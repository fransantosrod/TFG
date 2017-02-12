#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main (){
	
	FILE *fichero;
	
	//Variable para leer el fichero caracter a carater
	char c;
	//Variable para almacenar el valor del caracter anterior
	char c_ant;
	
	//Variable para contabilizar el número de frases del fichero
	int frase;
	//Variable para contabilizar en que palabra estamos
	int palabra;
	//Variable para contabilizar los parrafos
	int parrafos;
	//Variable para contabilizar el número de letras
	int letra;
	//Bandera para bloquear el aumento de frases si estamos leyendo un salto de parrafo
	bool flag;
	
	//Array auxiliar para almacenar cada palabra de la frase
		//200 caracteres por palabras
	char *palabras_frase = (char *)malloc (sizeof(char)*200);
	//Array para almacenar el contenido del documento
		//500 líneas y 50 palabras por líneas
	char *contenido[500][50];
	//Variable auxiliar para almacenar cuantas palabras tiene cada frase
		//50 igual que en el caso anterior
	int cont_aux_pal[50];
	//Inicialización de variables
	fichero = fopen("alert_prueba", "r");
	frase = 0;
	c_ant = ' ';
	parrafos=0;
	palabra = 0;
	letra = 0;
	flag = false;
	
	//Comenzamos a leer el fichero
	while ((c=fgetc(fichero))!=EOF){
		
		if (c!= ' ' && c!='\n'){
			palabras_frase[letra] = c;
			//Aumentamos el número de letras leidas
			letra++;
		}
		
		//Comprobamos si el caracter leido anteriormente y el actual son un salto de linea
		else if (c_ant == '\n' && c == '\n'){

			//Si lo es, estamos en un nuevo parrafo
			flag=true;
			palabra=0;
			parrafos++;
			
		}
		
		//Comprobamos si se ha leido alguna letra,si esta esta es un salto de línea y no estamos en el salto de parrafo 
		else if (c == '\n' && letra>0 && flag!= true){
			
			//Almacenamos la frase deseada en nuestra tabla
			palabras_frase[letra++] = '\0';
			contenido[frase][palabra] = strdup(palabras_frase);
			//printf("%s\n", contenido[frase][palabra]);
			
			//Volvemos a iniciar el número de letras ya que estamos en una nueva frase y palabra
			letra = 0;
			cont_aux_pal[frase]=palabra;
			palabra=0;	
			
			//En ese caso aumentamos el número de frases leidas
			frase++;
			
		}
		
		//Comprobamos si el caracter es un espacio y si se ha leido alguno 
		else if (c == ' ' && letra >0){
			
			//Almacenamos la frase deseada en nuestra tabla
			palabras_frase[letra++] = '\0';
			contenido[frase][palabra] = strdup(palabras_frase);
			//printf("%s\n", contenido[frase][palabra]);
			
			//Aumentamos el número de palabras leidas
			palabra++;
			//Volvemos a iniciar el número de letras leidas ya que estamos en una nueva palabra
			letra=0;
		}

		c_ant=c;;
		flag=false;
		
	}
	int i = 0;
	int j = 0;

	for (i=0;i<frase;i++){
		for (j=0; j<=cont_aux_pal[i];j++){
			printf("%s ", contenido[i][j]);
		}
		printf("\n");
	}	
	fclose(fichero);
	//Liberamos la memoria
	free(palabras_frase);

	
	return 0;
} 	
