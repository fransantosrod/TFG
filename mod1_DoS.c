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

	//Árray doble para almacenar lo que se ha leido del fichero
	char *contenido = (char *)malloc(sizeof(char)*1000);
	//Array para almacenar cada palabra de la frase
	char *palabras_frase = (char *)malloc (sizeof(char)*100);

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
			
			/**/
			palabras_frase[letra++] = '\0';
			printf("%s\n", palabras_frase);
			printf("\n");
			memcpy(&contenido[0], palabras_frase, letra);
			printf("%c\n", contenido[0]);
			printf("\n");
			/**/
			//En ese caso aumentamos el número de frases leidas
			frase++;
			
			//Volvemos a iniciar el número de letras ya que estamos en una nueva frase y palabra
			letra = 0;
			
		}
		
		//Comprobamos si el caracter es un espacio y si se ha leido alguno 
		else if (c == ' ' && letra >0){
			
			/**/
			palabras_frase[letra++] = '\0';
			//printf("%s ", palabras_frase);
			/**/
			//Aumentamos el número de palabras leidas
			palabra++;
			//Volvemos a iniciar el número de letras leidas ya que estamos en una nueva palabra
			letra=0;
		}
		
		c_ant=c;;
		flag=false;
		
		//printf ("%c", c);
		
	}
	
	fclose(fichero);
	//Liberamos la memoria
	free(palabras_frase);
	free(contenido);
	
	printf("\n");
	printf("%d\n", parrafos);
	printf("%d\n", frase);
	printf("%d\n", palabra);
	
	return 0;
} 	
