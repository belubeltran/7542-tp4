//
//  server_oyente.h
//  CLASE OYENTE
//  


#ifndef OYENTE_H
#define OYENTE_H

#include <iostream>
#include <list>

#include "common_thread.h"
#include "common_socket.h"
#include "server_conexion_cliente.h"
#include "server_asignador_tarea.h"



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Oyente : public Thread {
private:

	int puerto;									// Puerto en el que se escucha.
	Socket socket;								// Socket en el que escucha el 
												// servidor.
	std::list<ConexionCliente*> *clientes;		// Lista de clientes conectados
	int& cantClientesConectados;				// Número de clientes conectados
	AsignadorTarea *asignadorTarea;				// Asignador de tareas

public:

	// Constructor
	Oyente(int puerto, std::list<ConexionCliente*> *clientes, 
		int& cantClientesConectados, AsignadorTarea *asignadorTarea);

	// Destructor
	~Oyente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Detiene la ejecución del oyente
	void detener();
};

#endif
