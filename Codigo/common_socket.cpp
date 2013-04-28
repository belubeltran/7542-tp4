// 
//  common_socket.cpp
//  CLASE SOCKET
// 
//  Clase que implementa la interfaz de los sockets de flujo (utilizando el 
//  protocolo TCP), proporcionando un conjunto medianamente extenso de métodos
//  y propiedades para las comunicaciones en red. 
// 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

#include "common_socket.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor.
// Crea un nuevo socket.
// POST: ĺanza una excepción si no se ha podido crear el socket.
Socket::Socket() {
	// Creamos el socket
	if((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw "ERROR: No se ha podido crear el socket.";
}


// Constructor privado.
// Crea un nuevo socket.
// PRE: 'sockfd' es un filedescriptor que identifica a un socket.
Socket::Socket(const int sockfd) : sockfd(sockfd) { }


// Destructor. 
// Cierra el socket.
Socket::~Socket() {
	close(this->sockfd);
}


// Enlaza (asocia) al socket con un puerto y una dirección IP.
// PRE: 'ip' es una cadena que contiene el nombre del host o la dirección
// IP a la que se desea asociar; 'puerto' es el puerto al que se desea
// enlazar.
// POST: lanza una excepción si no se logra llevar a cabo el enlace.
void Socket::enlazar(std::string ip, int puerto) {
	// Obtenemos host
	struct hostent *he = gethostbyname(ip.c_str());

	// Cargamos datos del enlace a realizar
	this->miDir.sin_family = AF_INET;
	this->miDir.sin_port = htons(puerto);
	this->miDir.sin_addr = *((struct in_addr *)he->h_addr);
	memset(miDir.sin_zero, '\0', sizeof(miDir.sin_zero));

	// Enlazamos
	if(bind(this->sockfd, (struct sockaddr *)&miDir, sizeof(miDir)) < 0)
		throw "ERROR: No se pudo llevar a cabo el enlace.";
}


// Enlaza (asocia) al socket con un puerto. La dirección IP es escojida de
// forma automática tomando la de la máquina sobre la cual se está 
// ejecutando el proceso.
// PRE: 'puerto' es el puerto al que se desea enlazar.
// POST: lanza una excepción si no se logra llevar a cabo el enlace.
void Socket::enlazar(int puerto) {
	// Cargamos datos del enlace a realizar
	this->miDir.sin_family = AF_INET;
	this->miDir.sin_port = htons(puerto);
	this->miDir.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(miDir.sin_zero), '\0', sizeof(miDir.sin_zero));

	// Enlazamos
	if(bind(this->sockfd, (struct sockaddr *)&miDir, sizeof(miDir)) < 0)
		throw "ERROR: No se pudo llevar a cabo el enlace.";
}


// Conecta el socket a una dirección y puerto destino.
// PRE: 'hostDestino' es una cadena que contiene el nombre del host o la
// dirección IP a la que se desea conectar; 'puertoDestino' es el puerto 
// al que se desea conectar.
// POST: determina dirección y puertos locales si no se utilizó el método 
// bind() previamente. Además, lanza una excepción si no se pudo llevar a
// cabo la conexión.
void Socket::conectar(std::string hostDestino, int puertoDestino) {
	// Obtenemos host
	struct hostent *he = gethostbyname(hostDestino.c_str());

	// Cargamos datos de la conexión a realizar
	destinoDir.sin_family = AF_INET;
	destinoDir.sin_port = htons(puertoDestino);
	// destinoDir.sin_addr.s_addr = inet_addr(ipDestino.c_str());
	destinoDir.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(destinoDir.sin_zero), '\0', sizeof(destinoDir.sin_zero));

	// Conectamos
	if(connect(this->sockfd, (struct sockaddr *)&destinoDir, 
		sizeof(struct sockaddr)) == -1)
		throw "ERROR: No se pudo llevar a cabo la conexión.";
}


// Configura el socket para recibir conexiones en la dirección y puerto 
// previamente asociados mediante el método enlazar();
// PRE: 'maxConexiones' es el número de conexiones entrantes permitidas en
// la cola de entrada.
// POST: lanza una excepción si no se pudo inicializar la escucha.
void Socket::escuchar(int maxConexiones) {
	if(listen(this->sockfd, maxConexiones) == -1)
		throw "ERROR: No se pudo comenzar a escuchar.";
}


// Espera una conexión en el socket previamente configurado con el método
// escuchar().
// POST: lanza una excepción si no pudo aceptar la conexión.
Socket* Socket::aceptar() {
	unsigned sin_size = sizeof(struct sockaddr_in);
	int sCliente = accept(sockfd, (struct sockaddr *)&destinoDir, &sin_size);

	// Corroboramos si se produjo un error
	if(sCliente < 0)
		throw "ERROR: No se pudo aceptar la conexión";

	return (new Socket(sCliente));
}


// Envía datos a través del socket.
// PRE: 'dato' es el dato que se desea enviar; 'longDato' es la longitud 
// de los datos en bytes.
// POST: devuelve el número de bytes que han sido enviados. Además, se 
// lanza una excepción si no se pudo concretar el envio de datos.
int Socket::enviar(const void* dato, int longDato) {
	// Realizamos el envío	
	int numBytes = send(this->sockfd, dato, longDato, 0);

	// Corroboramos si se produjo un error
	if(numBytes == -1)
		throw "ERROR: No se pudo realizar el envío de datos.";

	return numBytes;
}


// Recibe datos a través del socket.
// PRE: 'buffer' es el buffer en donde se va a depositar la información 
// leida; 'longBuffer' es la longitud máxima del buffer.
// POST: devuelve el número de bytes que han sido leidos o 0 (cero) si el
// host remoto a cerrado la conexión. Además, lanza una excepción si no se
// pudo concretar la recepción de datos.
int Socket::recibir(void* buffer, int longBuffer) {
	// Realizamos la recepción
	int numBytes = recv(this->sockfd, buffer, longBuffer, 0);

	// Corroboramos si se produjo un error
	if(numBytes == -1)
		throw "ERROR: No se pudo realizar el envío de datos.";

	return numBytes;
}


// Cierra el socket. Brinda distintos tipos de formas de cerrar permitiendo
// realizar un cierre del envío y recepción de datos en forma ordenada.
// PRE: si 'modo' es 0, no se permite recibir más datos; si es 1, no se 
// permite enviar más datos; si es 2, no se permite enviar ni recibir más
// datos, quedando inutilizable el socket. Si no se especifica ningún modo 
// al llamar al método, se utiliza por defecto el modo 2.
// POST: el socket quedará parcial o completamente inutilizable 
// dependiendo del modo elegido.
int Socket::cerrar(int modo) {
	return shutdown(this->sockfd, modo);
}

