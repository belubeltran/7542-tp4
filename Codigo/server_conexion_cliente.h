//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"
#include "server_asignador_tarea.h"



class ConexionCliente : public Thread {
private:

	Socket *socket;								// Socket de comunicación
	int id;										// Número de cliente
	AsignadorTarea *asignadorTarea;				// Asignador de tareas

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	ConexionCliente(Socket *s, int id, AsignadorTarea *asignadorTarea);

	// Destructor
	~ConexionCliente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();
};

#endif
