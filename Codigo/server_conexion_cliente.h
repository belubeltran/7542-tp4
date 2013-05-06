//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"
#include "server_asignador_tarea.h"
#include "common_lista.h"



class ConexionCliente : public Thread {
private:

	int id;										// Número de cliente
	Socket *socket;								// Socket de comunicación
	Lista<ConexionCliente*> *clientes;			// Lista de clientes conectados
	Lista<std::string> *claves;					// Lista de posibles claves.
	AsignadorTarea *asignadorTarea;				// Asignador de tareas

	// Recibe un mensaje entrante
	// POST: devuelve un string con el mensaje recibido
	std::string recibirMensaje();

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	ConexionCliente(Socket *s, Lista<ConexionCliente*> *clientes, 
		Lista<std::string> *claves, AsignadorTarea *asignadorTarea);

	// Destructor
	~ConexionCliente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Asigna un número de identificación a la conexión
	void asignarID(int id);

	// Devuelve la identificación de la conexión. Si no posee identificación,
	// devuelve -1.
	int ID();
};

#endif
