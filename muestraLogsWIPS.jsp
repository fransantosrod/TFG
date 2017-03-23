<!--
	Fecha: 21-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que muestra los
	logs generados por el WIPS
-->

<!-- Indicamos que vamos a introducir código en Java -->
<%@ page language="java" import="java.io.*, java.net.*" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<!DOCTYPE html>
<html>
	
	<head>
		<!--
			Añadimos el logo en la cabecera
		-->
		<link rel="icon" type="image/png" href="../cgi/logo.png"/>
		<!--
			Definimos en la cabecera el título de la página 
			y la hoja de estilos que se aplicará
		-->
		<link rel="stylesheet" type="text/css" href="../cgi/hoja_estilo.css">
		<title> Gesti&oacuten WIPS </title>
	</head>
	
	<body>
		<!--
			CREAMOS LA BOTONERA SUPERIOR
		-->
		<p>
		<!-- 
			Indicamos la dirección a la que nos dirigimos si se pulsa 
			ese botón, el método con el que se realizará la petición y
			el estilo del botón, que será en línea para que todos aparezcan
			en una única fila  en la parte superior
		-->
			<form method="get" action="../cgi/wips.html" style="display:inline">
				
				<button type="submit">
					Inicio
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/cgi-bin/muestraReglasSnort.out" style="display:inline">
			
				<button type="submit">
					Reglas Snort
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/cgi-bin/muestraReglasIptables.out" style="display:inline">
			
				<button type="submit">
					Reglas Iptables
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/jsp/muestraClientesRegistrados.jsp" style="display:inline">
			
				<button type="submit">
					Clientes Registrados
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/cgi-bin/muestraRegistroReglasMITM.out" style="display:inline">
			
				<button type="submit">
					Registro Reglas MITM
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/jsp/muestraLogsWIPS.jsp" style="display:inline">
			
				<button type="submit">
					Logs WIPS
				</button>
			
			</form>

			<form method="get" action="http://localhost/dit/cgi-bin/borraRegla.out" style="display:inline">
			
				<button type="submit">
					Borrar Reglas
				</button>
			
			</form>

		</p>
		
		<p>
			<form method="get" action="http://localhost/dit/jsp/muestraLogsWIPS.jsp">

				<button type="submit" class="button1">
					Actualizar logs
				</button>
				<div class="muestraLogs">
					
					<%
						//Ruta relativa del fichero de loqs del WIPS	
						String fichero = "/home/dit/web/cgi/wips.log";

						//Creamos el objeto para leer el fichero
						BufferedReader lectura = new BufferedReader(new FileReader(fichero));
						String linea;
						int lineas_leidas = 0;

						//Leemos el fichero línea por línea
						while((linea = lectura.readLine()) != null) {

							//Imprimimos las líneas leídas
							out.println("<p class=\"logs\">");
							out.println(linea);
							out.println("</p>");
							
							//Aumentamos el contador de líneas leídas	
							lineas_leidas++;
						}

						//Si no se ha leido ninguna línea, lo indicamos
						if (lineas_leidas == 0) {
							out.println("<p class=\"log_vacio\"> El fichero de logs est&aacute vac&iacuteo </p>");
						}
						
					%>	
				</div>
			</form>
		</p>
	</body>

</html>