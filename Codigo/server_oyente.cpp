//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  


#include "server_oyente.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Oyente::Oyente() {

}

// Destructor
Oyente::~Oyente() {

}


// Define tareas a ejecutar en el hilo.
void Oyente::run() {
	// Iniciamos la escucha del servidor
	this->socket.enlazar(this->puerto);
	this->socket.escuchar(MAX_CONEXIONES);
	// std::cout << "Servidor escuchando..." << std::endl;

	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		
		Socket *socketCLI = 0;

		//Aceptamos nuevo cliente
		try {
			socketCLI = this->socket.aceptar();
		}
		catch (const char* e) {
			// Si el socket no esta activo salteamos instrucciones
			// siguientes
			if(!this->socket.estaActivo()) break;
		}

		// Generamos una nueva conexión para este
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
			this->cantClientesConectados);

		// Censamos al cliente en el servidor
		this->clientes.push_back(conexionCLI);
		this->cantClientesConectados++;

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
	}
}
