//
//  client_cliente.h
//  CLASE CLIENTE
//  


#ifndef CLIENTE_H
#define CLIENTE_H


#include "common_socket.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Cliente{
private:

	Socket socket;						// Socket con el que se comunica
	int puerto;							// Puerto de conexión.
	std::string nombreHost;				// Nombre del host de conexión

	// Recibe un mensaje entrante
	// POST: devuelve un string con el mensaje recibido
	std::string recibirMensaje();

	// Prueba una a una las claves en el código Draka y envía al servidor
	// aquellas claves que pasen la prueba.
	void procesarClaves(std::string msgEncriptado, int numDig, 
		int claveIni, int claveFin);

public:

	// Constructor
	Cliente(std::string nombreHost, int puerto);

	// Destructor
	~Cliente();

	// Mantiene la comunicación con el servidor.
	void ejecutar();
};

#endif
