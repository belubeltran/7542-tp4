//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"


class ConexionCliente : public Thread {
private:

	// Servidor *miServidor;		// Servidor al que pertenece la conexión
	Socket *socket;				// Socket de transmisión de datos
	int id;						// Número de cliente

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	ConexionCliente(Socket *s, int id);

	// Destructor
	~ConexionCliente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();
};

#endif
