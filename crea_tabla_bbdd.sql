CREATE TABLE datos_clientes
(
dir_mac macaddr NOT NULL,
dir_ip inet NOT NULL,
fecha timestamp without time zone NOT NULL,
CONSTRAINT primary_key PRIMARY KEY (dir_mac, fecha) 
);
