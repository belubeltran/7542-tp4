//
//  server_oyente.cpp
//  CLASE OYENTE
//  
//	Clase que se encarga de crear el socket que se mantiene a la escucha de 
//  nuevas conexiones de clientes
//


#include "server_oyente.h"


// Constantes
namespace {
	const int MAX_CONEXIONES = 10;
}



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Oyente::Oyente(int puerto, std::list<ConexionCliente*> *clientes, 
		int& cantClientesConectados, AsignadorTarea *asignadorTarea) : 
		puerto(puerto), clientes(clientes),	
		cantClientesConectados(cantClientesConectados), 
		asignadorTarea(asignadorTarea) { }

// Destructor
Oyente::~Oyente() { }


// Define tareas a ejecutar en el hilo.
void Oyente::run() {
	// Iniciamos la escucha del servidor
	this->socket.enlazar(this->puerto);
	this->socket.escuchar(MAX_CONEXIONES);
	std::cout << "Servidor escuchando..." << std::endl;

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
			this->cantClientesConectados, this->asignadorTarea);

		// Censamos al cliente en el servidor
		this->clientes->push_back(conexionCLI);
		this->cantClientesConectados++;

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
		std::cout << "Hay " << this->cantClientesConectados << " ahora conectados." << std::endl;
	}
}


// Detiene la ejecución del oyente
void Oyente::detener() {
	this->stop();
	this->socket.cerrar();
}
