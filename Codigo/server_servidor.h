//
//  server_servidor.h
//  CLASE SERVIDOR
//  


#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>

#include "common_thread.h"
#include "common_socket.h"
#include "server_oyente.h"
#include "server_asignador_tarea.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Servidor : public Thread {
private:

	std::list<ConexionCliente*> *clientes;		// Lista de clientes conectados
	int puerto;									// Puerto en el que se escucha.
	int numDigitosClave;						// Número de dígitos de la
												// clave.
	int numClientes;							// Número de clientes entre los
												// que se dividira el trabajo.
	int cantClientesConectados;					// Número de clientes
												// conectados
	std::string msgEncriptado;					// Mensaje encriptado en hexa
	AsignadorTarea *asignadorTarea;				// Asignador de tareas
	Oyente *oyente;								// Oyente de conexiones

public:

	// Constructor
	Servidor(int puerto, const  std::string& archivo, int numDigitosClave, 
		int numClientes);

	// Destructor
	~Servidor();

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clients.
	virtual void run();

	// Detiene la ejecución del servidor
	void detener();
};

#endif
