<%@ page language="java" import="java.sql.*" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<%
	try {

		Class.forName("org.postgresql.Driver").newInstance();
		
		String url = "jdbc:postgresql://localhost:5432/clientes_asociados";
		String user = "clientes";
		String pass = "clientes";

		Connection conn = DriverManager.getConnection(url,user,pass);

		Statement st = conn.createStatement();
		ResultSet rs = st.executeQuery("SELECT * FROM datos_clientes");
	

%>

<!DOCTYPE html>
<!DOCTYPE html>
<html>
	
	<head>
		
		<link rel="stylesheet" type="text/css" href="../cgi/hoja_estilo.css">
		<title> Gestion WIPS </title>

	</head>
	
	<body>
			<p>

			<form method="get" action="../cgi/prueba.html" style="display:inline">

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

			<form method="get" action="http://localhost/dit/cgi-bin/borraRegla.out" style="display:inline">
			
				<button type="submit">
					Borrar Reglas
				</button>
			
			</form>

		</p>

		<form  method="get" action="http://localhost/dit/jsp/muestraClientesRegistrados.jsp" style="display:inline">

				

			<table class="tabla_clientes">
				<tr>
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
					while (rs.next()){
					
						out.println("<tr>");
							
							out.println("<td>" + rs.getObject(1) + "</td>");
							out.println("<td>" + rs.getObject(2) + "</td>");
							out.println("<td>" + rs.getObject(3) + "</td>");
							
						out.println("</tr>");
				%>

				
				<%
					}
						rs.close();
						st.close();
						conn.close();

					} catch (SQLException e) {

							out.println("Excepcion SQL Exception: " + e.getMessage());
							e.printStackTrace();
						}
				%>
			</table>
			
			<button type="submit" class="button1">
				Actualizar registro
			</button>	
			
			
		</form>

		<form method="get" action="http://localhost/dit/jsp/muestraClientesRegistrados.jsp">
			
			<p class="filtro">
			
				Direccion MAC:
				<input type="text" name="macaddr">
				
				Direccion IP:
				<input type="text" name="ip">

			</p>
		

			<button type="submit" class="button1" style="display;inline;position:absolute;left: 61%;top:116px;" ">
				Filtar
			</button>

		
		</form>
		
	</body>

</html>