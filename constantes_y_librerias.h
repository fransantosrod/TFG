/*-------------------------------------------
Autor: Francisco Javier Santos
Fecha: 13-03-2017
Descripción: Fichero que contiene todas las
constantes y librerias necesarias para 
la ejecución del programa
-------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

#ifndef constantes_y_librerias
#define constantes_y_librerias


/*-----------------------------------------------------------------------
	Constantes para definir las rutas y nombres de los ficheros que se
	va a usar en las funciones
-----------------------------------------------------------------------*/
#define FICHERO_ALERT_SNORT "alert"
#define FICHERO_REGLAS_SNORT "local.rules_prueba"
#define FICHERO_SID "sid"
#define FICHERO_PID_SNORT "pid.txt"
#define FICHERO_ARPWATCH "arpwatch_prueba.log"
#define FICHERO_REGISTRO_REGLAS_MITM "registro_reglas_MITM"
#define FICHERO_CAPTURA_WIFI "primera_captura_prueba.csv"
#define FICHERO_CAPTURA_WIFI_SIN_EXTENSION "primera_captura_prueba"
#define EXTENSION_FICHERO_CAPTURA_WIFI "-01.csv"
#define FICHERO_PID_AIRODUMP "pid_airodump.txt"
#define FICHERO_REGISTRO_CLIENTES "registro_clientes.txt"


//Constante para definir la contrasenia de la base de datos
#define CONTRASENIA_BBDD "clientes"
//Constante para definir el usuario con el que nos queremos registrar
#define USUARIO_BBDD  "clientes"
//Constante para definir la bbdd a la que deseamos acceder
#define NOMBRE_BBDD "clientes_asociados"
//Constante para definir la tabla a la que vamos a acceder
#define NOMBRE_TABLA_BBDD "datos_clientes"


#define INICIO 0
#define IGUAL 0
//Constante que devolverá la funcion busca_Regla cuando no encuentre una igual
#define NO_COINCIDE -1
#define NO_EXISTE -1


/*-------------------------------------------------------
	Constante para definir el número máximo de línea que
	contiene el fichero, sin contar con las líneas
	en blanco 
-------------------------------------------------------*/
#define NUM_FRASES 500
//Constante para definir el límite a partir del cuál crearemos un nuevo fichero (en mod1_DoS.c)
#define LIMITE_LINEAS_LEIDAS NUM_FRASES-30
//Constante para definir el número máximo de palabras en una frae
#define NUM_PALABRAS_POR_FRASE 50
//Constante para definir el número máximo de caracteres para una palabra
#define NUM_CARACTERES_PALABRA 100
//Número máximo de reglas que almacenarán las tablas
#define NUM_REGLAS 100
//Constante para definir el número máximo de caracteres para una regla
#define NUM_CARACTERES_REGLAS 500
//Constante para definir el número máximo de digitos que puede tener el SID
#define NUM_DIG_SID 10
//Constante para definir el número máximo de SSID que vamos a almacenar en la estructura
#define NUM_SSID 50


#define TABULADOR '\t'
#define SALTO_DE_LINEA '\n'
#define ESPACIO ' '
#define PUNTO '.'
#define TERMINADOR '\0'
//Constante para definir el caracter :
#define DOS_PUNTOS ':'
//Constante para definir el caracter ','
#define COMA ','


//Constates para definir la posición dentro de una tabla de cada fragmento
#define FRAG_1 0
#define FRAG_2 1
#define FRAG_3 2
//Constantes para definir el valor de cada fragmento de IP de la red local
#define VALOR_FRAG_1 "10"
#define VALOR_FRAG_2 "10"
#define VALOR_FRAG_3 "10"
//El número máximo de digitos que puede tener un fragmento de IP
#define MAX_DIG_FRAGMENTO_IP 3


//Constante para definir los protocolos
#define UDP "UDP"
#define TCP "TCP"
#define ICMP "ICMP"
#define IP "IP"


//Constante para definir la línea de fin de una coincidencia
#define FIN_REGLA_ALERTA 3
#define SIGUIENTE_LINEA 1
//Constante para definir la posición relativa del protocolo
#define LINEA_PROTOCOLO 2
//Constante donde definimos la posición de la hora en el fichero de registro
#define HORA 0
//Constante para indicar la posición de la fecha en la bbdd
#define FECHA 2
//Constante para definir la posición de la IP origen dentro de una línea
#define POSICION_IP_ORIGEN 3
//Igual que el anterior pero relativa a la IP destino
#define POSICION_IP_DESTINO 5
//Constantes para definir la posición que ocupan los campos relevantes en una regla
#define POS_ACCION 0
#define POS_PROTOCOLO 1
#define POS_DIR_IP_ORIG 2
#define POS_PUERTO_ORIG 3
#define POS_DIR_IP_DEST 5
#define POS_PUERTO_DEST 6
//Posición que ocupa la dirección IP
#define POS_DIR_IP 3
#define POS_DIR_IP_INM POS_DIR_IP-1
//Posición que ocupa la nueva dirección MAC
#define POS_DIR_MAC_NUEVA 4
#define POS_DIR_MAC_NUEVA_INM POS_DIR_MAC_NUEVA-1
//Posición que ocupa la vieja dirección MAC
#define POS_DIR_MAC_VIEJA 5
#define POS_DIR_MAC_VIEJA_INM POS_DIR_MAC_VIEJA-1
//Constante para definir la posición de la bandera que indica si la IP es origen o destino
#define POS_BANDERA_IP 4
//Constante para definir la posición de la bandera que indica si la IP lleva puerto asociado
#define POS_BANDERA_PUERTO 5


//Constante para definir el número máximo de digitos de un puerto
#define TAM_PUERTO 6
//Constante para definir el número máximo de digitos que puede tener la fecha
#define TAM_FECHA 100
//Tamaño de la dirección MAC
#define TAM_MAC 17
//Constante para definir el número máximo de octetos en una dirección IP
#define TAM_IP 4


//Constante para definir el valor inicial del semáforo
#define INICIO_SEM 1
//Constante para almacenar el nombre del semáforo para los ficheros de lecturas
#define SEM_LECTURA "semaforo_lectura"
//Constante para definir el nombre del semáforo que se usará para la recarga de Snort
#define SEM_SNORT "semaforo_snort"
//Constante para definir el nombre del semáforo que usará el fichero donde se almacena el sid
#define SEM_SID "semaforo_sid"
//Constante para definir el nombre del semáforo que usará el fichero de reglas de snort
#define SEM_REGLAS_SNORT "semaforo_reglas_snort"


//Constante de tiempo para definir los segundos que pasaran entre actividad
#define INTERVALO_LECTURA 3
//Constante paara definir el paso de una hora
#define PASA_UNA_HORA 100
//Constante para definir el tiempo en el que estaremos capturando tráfico de la red
#define TIEMPO_CAPTURA 30


//Indica que la dirección MAC ha cambiado para una misma IP
#define CAMBIO_EN_MAC "changed"
//Indica que la MAC ha cambiado en un intervalo muy corto de tiempo 
#define CAMBIO_MAC_INMEDIATO "flip"


//Constante para definir la palabra ESSID
#define ESSID "ESSID"
//Connstante para definir la palabra BSSID
#define BSSID "BSSID"
//Constante para definir la palabra que indica que comienza la info de los clientes
#define INFO_CLIENTES "Station MAC"


#endif