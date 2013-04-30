//
//  common_servidor.h
//  CLASE SERVIDOR
//  
//  
//  



#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <list>

#include "common_thread.h"
#include "common_socket.h"
#include "common_conexion_cliente.h"



class Servidor : public Thread {
private:

	std::list<ConexionCliente*> clientes;		// Lista de clientes conectados
	Socket socket;								// Socket en el que escucha el 
												// servidor
	int contIdCliente;

public:

	// Constructor
	Servidor(int puerto);

	// 
	virtual void run();
};

#endif
