//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   


#include <iostream>
#include "server_conexion_cliente.h"




// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
ConexionCliente::ConexionCliente(Socket *s, int id) : 
	socket(s), id(id) { }


// Destructor
ConexionCliente::~ConexionCliente() {
	this->socket->cerrar();
}


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	
	while(this->isActive()) {
		char buf[100];

		this->socket->recibir(buf, 100-1);

		if(buf[0] == 'q') {
			this->socket->cerrar();
			stop();
			std::cout << "Se desconectó cliente " << this->id << std::endl;
			break;
		}

		this->socket->enviar(buf, 100-1);
		std::cout << "Echo al cliente " << this->id << std::endl;
	}
}
