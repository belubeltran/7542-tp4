//
//  client_cliente.h
//  CLASE CLIENTE
//  
//  
//  


#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include "common_thread.h"
#include "common_socket.h"



class Cliente : public Thread {
private:

	Socket socket;								// Socket con el que se comunica
	int puerto;									// Puerto de conexión.
	std::string nombreHost;						// Nombre del host de conexión

	// Prueba una a una las claves en el código Draka y envía al servidor
	// aquellas claves que pasen la prueba.
	void procesarClaves(std::string& claveMin, std::string& claveMax);

public:

	// Constructor
	Cliente(std::string nombreHost, int puerto);

	// Destructor
	~Cliente();

	// Define tareas a ejecutar en el hilo.
	// Mantiene la comunicación con el servidor.
	virtual void run();
};

#endif
