//
//  common_servidor.cpp
//  CLASE SERVIDOR
//  
//  
//  


#include "common_servidor.h"

#include <iostream>


Servidor::Servidor(int puerto) : contIdCliente(1){
	this->socket.enlazar(puerto);
	this->socket.escuchar(5);
	std::cout << "Servidor escuchando..." << std::endl;
}

void Servidor::run() {

	int i = 1;

	while(true) {
		Socket *socketCLI = this->socket.aceptar();
		// std::cout << "Se conectó el cliente " << this->contIdCliente << std::endl;
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI, i);
		i++;
		this->clientes.push_back(conexionCLI);
		conexionCLI->start();
	}

	// std::list<ConexionCliente*>::iterator i;
	// for(i = this->clientes->begin(); i != this->clientes->end(); ++i)
	// 	(*i)->join();
}

