//
//  server_servidor.h
//  CLASE SERVIDOR
//  


#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <list>

#include "common_thread.h"
#include "common_mutex.h"
#include "common_socket.h"
#include "server_conexion_cliente.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


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
	int cantClientesConectados;					// Número de clientes conectados
	std::string msgEncriptado;					// Mensaje encriptado en hexadecimal
	Mutex m;									// Mutex


	// Devuelve la clave inicial del espacio de claves a asignar a un cliente.
	// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
	// POST: se devuelve una cadena con la clave inicial.
	std::string claveInicialDeRangoDeClaves(int numCliente);

	// Devuelve la clave final del espacio de claves a asignar a un cliente.
	// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
	// POST: se devuelve una cadena con la clave final.
	std::string claveFinalDeRangoDeClaves(int numCliente);


public:

	// Constructor
	Servidor(int puerto, const  std::string& archivo, int numDigitosClave, 
		int numClientes);

	// Destructor
	~Servidor();

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clients.
	virtual void run();

	// Termina la ejecución del servidor
	void apagar();
};

#endif
