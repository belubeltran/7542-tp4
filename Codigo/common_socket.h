//  
//  common_socket.h
//  CLASE SOCKET
//  
//  Clase que implementa la interfaz de los sockets de flujo (utilizando el 
//  protocolo TCP), proporcionando un conjunto medianamente extenso de métodos
//  y propiedades para las comunicaciones en red. 
//  


#ifndef SOCKET_H
#define SOCKET_H


#include <netinet/in.h>
#include <string>




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Socket {
private:

	int sockfd;							// Filedescriptor del socket.
	struct sockaddr_in miDir;			// Dirección del socket.
	struct sockaddr_in destinoDir;		// Dirección del socket destino.
	bool activo;						// Sensa si esta activo el socket

	// Constructor privado.
	// Crea un nuevo socket.
	// PRE: 'sockfd' es un filedescriptor que identifica a un socket.
	explicit Socket(const int sockfd);

public:

	// Constructor.
	// Crea un nuevo socket.
	// POST: ĺanza una excepción si no se ha podido crear el socket.
	Socket();

	// Destructor. 
	// Cierra el socket.
	~Socket();

	// Enlaza (asocia) al socket con un puerto y una dirección IP.
	// PRE: 'ip' es una cadena que contiene el nombre del host o la dirección
	// IP a la que se desea asociar; 'puerto' es el puerto al que se desea
	// enlazar.
	// POST: lanza una excepción si no se logra llevar a cabo el enlace.
	void enlazar(std::string host, int puerto);

	// Enlaza (asocia) al socket con un puerto. La dirección IP es escojida de
	// forma automática tomando la de la máquina sobre la cual se está 
	// ejecutando el proceso.
	// PRE: 'puerto' es el puerto al que se desea enlazar.
	// POST: lanza una excepción si no se logra llevar a cabo el enlace.
	void enlazar(int puerto);

	// Conecta el socket a una dirección y puerto destino.
	// PRE: 'hostDestino' es una cadena que contiene el nombre del host o la
	// dirección IP a la que se desea conectar; 'puertoDestino' es el puerto 
	// al que se desea conectar.
	// POST: determina dirección y puertos locales si no se utilizó el método 
	// bind() previamente. Además, lanza una excepción si no se pudo llevar a
	// cabo la conexión.
	void conectar(std::string hostDestino, int puertoDestino);

	// Configura el socket para recibir conexiones en la dirección y puerto 
	// previamente asociados mediante el método enlazar();
	// PRE: 'maxConexiones' es el número de conexiones entrantes permitidas en
	// la cola de entrada.
	// POST: lanza una excepción si no se pudo inicializar la escucha.
	void escuchar(int maxConexiones);

	// Espera una conexión en el socket previamente configurado con el método
	// escuchar().
	// POST: lanza una excepción si no pudo aceptar la conexión.
	Socket* aceptar();

	// Envía datos a través del socket.
	// PRE: 'dato' es el dato que se desea enviar; 'longDato' es la longitud 
	// de los datos en bytes.
	// POST: devuelve el número de bytes que han sido enviados. Además, se 
	// lanza una excepción si no se pudo concretar el envio de datos.
	int enviar(const void* dato, int longDato);

	// Recibe datos a través del socket.
	// PRE: 'buffer' es el buffer en donde se va a depositar la información 
	// leida; 'longBuffer' es la longitud máxima del buffer.
	// POST: devuelve el número de bytes que han sido leidos o 0 (cero) si el
	// host remoto a cerrado la conexión. Además, lanza una excepción si no se
	// pudo concretar la recepción de datos.
	int recibir(void* buffer, int longBuffer);

	// Cierra el socket. Brinda distintos tipos de formas de cerrar permitiendo
	// realizar un cierre del envío y recepción de datos en forma ordenada.
	// PRE: si 'modo' es 0, no se permite recibir más datos; si es 1, no se 
	// permite enviar más datos; si es 2, no se permite enviar ni recibir más
	// datos, quedando inutilizable el socket. Si no se especifica ningún modo 
	// al llamar al método, se utiliza por defecto el modo 2.
	// POST: el socket quedará parcial o completamente inutilizable 
	// dependiendo del modo elegido.
	int cerrar(int modo = 2);

	// Corrobora si el socket se encuentra activo. Que no este activo significa
	// da cuenta de que el socket se encuentra inutilizable para la transmisión
	// y recepción de datos.
	// POST: devuelve true si el socket se encuentra activo o false en su
	// defecto. 
	bool estaActivo();
};

#endif
