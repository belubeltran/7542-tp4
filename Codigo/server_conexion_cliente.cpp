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
ConexionCliente::ConexionCliente(Socket *s, int id, 
	AsignadorTarea *asignadorTarea) : socket(s), id(id),
	asignadorTarea(asignadorTarea) { }


// Destructor
ConexionCliente::~ConexionCliente() {
	this->socket->cerrar();
}


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	
	while(this->isActive()) {
		char buf[100];

		this->socket->recibir(buf, 100-1);
		std::string msg = this->asignadorTarea->darIndicacion();
		this->socket->enviar(msg.c_str(), msg.size());
		std::cout << "Echo al cliente " << this->id << std::endl;
	}
}
