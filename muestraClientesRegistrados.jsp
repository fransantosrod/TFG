<!--
	Fecha: 19-03-2017
	Autor: Francisco Javier Santos Rodríguez
	Descripción: Página que se encarga de mostrar
	la información de la BBDD de los clientes registrados
	y permite filtrarlos
-->

<!-- Indicamos que vamos a introducir código en Java -->
<%@ page language="java" import="java.sql.*, java.io.*, java.util.*" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<%
	try {

		Class.forName("org.postgresql.Driver").newInstance();
		
		//Indicamos los parámetros para conectarnos a la BBDD
		String url = "jdbc:postgresql://localhost:5432/clientes_asociados";
		String user = "clientes";
		String pass = "clientes";

		//Establecemos la conexión
		Connection conn = DriverManager.getConnection(url,user,pass);

		//Creamos el objeto para enviar sentencias
		Statement st = conn.createStatement();
	
%>

<!DOCTYPE html>
<html>
	
	<head>

		<!--
			Añadimos el logo en la cabecera
		-->
		<link rel="shortcut icon" type="image/png" href="../cgi/logo.png"/>
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


		<form  method="get" action="http://localhost/dit/jsp/muestraClientesRegistrados.jsp" style="display:inline">

				
		<!--
			Creamos la tabla donde se motrará la información de los clientes
		-->
			<table class="tabla_clientes">
				<tr>
					<!--
						Indicamos los valores de cabecera
					-->
					<th>
						Direccion MAC
					</th>
					<th>
						Direccion IP
					</th>
					<th>
						Fecha registro
					</th>
				</tr>
					
				<%
					/*------------------------------------------------
						Obtenemos los parámetros del formulario que 
						nos permite filtrar la información
					-------------------------------------------------*/		
					String mac = request.getParameter("macaddr"); 
					String ip = request.getParameter("ip");
					//Creamos la primera parte de la consulta
					String sentencia = "SELECT * from datos_clientes ";

					
					if (mac != null && ip != null ) {
						
						//Comprobamos si hemos introducido algún valor	
						if (ip.length() > 0 || mac.length()>0) {
							
							//Añadimos la condición
							sentencia+="where ";
						}
						
						//Si queremos filtrar por la MAC
						if (mac.length() > 0) {
							/*------------------------------------------
								Añadimos la condición para mostrar solo
								los que contienen esa dirección MAC
							-------------------------------------------*/
							sentencia+= "dir_mac='" + mac + "'";
						}

						//Si hemos introducido valores en ambos campos
						if (ip.length() > 0 && mac.length()>0){
							
							/*-------------------------------
								Añadimos "and" para indicar
								que vamos a introducir otra 
								condición
							-------------------------------*/
							sentencia+= " and ";
						}

						//Si hemos introducio algún valor en la IP
						if (ip.length() > 0) {
							
							//Añadimos esa condición
							sentencia+= "dir_ip ='" + ip + "'";
						}
					
					}
					//Añadimos el terminador
					sentencia+=";";

					//Realizamos la consulta y la almacenamos
					ResultSet rs = st.executeQuery(sentencia);
					
					//Recorremos el resultado obtenido
					while (rs.next()){
						
						//Incluimos en una fila nueva cada resultado
						out.println("<tr>");

							//Incluimos en cada celda el valor de cada colummna
							out.println("<td>" + rs.getObject(1) + "</td>");
							out.println("<td>" + rs.getObject(2) + "</td>");
							out.println("<td>" + rs.getObject(3) + "</td>");
							
						out.println("</tr>");
					}
						//Cerramos la conexión
						rs.close();
						st.close();
						conn.close();

					} catch (SQLException e) {

							//En caso de error, imprimimos el siguiente mensaje de error
							out.println("<p class = \"error_sql\">La consulta realizada es erronea, presione <b>Actualizar registro</b> para ver todos los registros</p>");
							
						}
				%>
			</table>
			
			<!--
				Botón para actualizar el registro de clientes
			-->
			<button type="submit" class="button1">
				Actualizar registro
			</button>	
			

			
		</form>



		<!--
			CREAMOS EL FORMULARIO QUE NOS PERMITIRÁ FILTRAR LA INFORMACIÓN DE LA BBDD EN FUNCIÓN DE LOS DATOS DE LOS CLIENTES
		-->
		<form method="get" action="http://localhost/dit/jsp/muestraClientesRegistrados.jsp">
			
			<!--
				Creamos los campos a rellenar del formulario para filtrar la información
			-->
			<p class="filtro">
				
				Direccion MAC:
				<input type="text" name="macaddr">
				
				Direccion IP:
				<input type="text" name="ip">

			</p>
		
			<!--
				Botón con el que enviaremos los datos de la consulta
			-->
			<button type="submit" class="button1" style="display;inline;position:absolute;left: 61%;top:116px;" ">
				Filtar
			</button>

		
		</form>
		
	</body>

</html>