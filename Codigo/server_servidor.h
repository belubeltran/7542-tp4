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
#include "server_conexion_cliente.h"



class Servidor : public Thread {
private:

	std::list<ConexionCliente*> clientes;		// Lista de clientes conectados
	Socket socket;								// Socket en el que escucha el 
												// servidor.
	int puerto;									// Puerto en el que se escucha.
	int numDigitosClave;						// Número de dígitos de la
												// clave.
	int numClientes;							// Número de clientes entre los
												// que se dividira el trabajo.
	std::string msgEncriptado;					// Mensaje encriptado en hexadecimal

public:

	// Constructor
	Servidor(int puerto, const  std::string& archivo, int numDigitosClave, 
		int numClientes);

	// Define tareas a ejecutar en el hilo.
	virtual void run();
};

#endif
