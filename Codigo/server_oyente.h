//
//  server_oyente.h
//  CLASE OYENTE
//  


#ifndef OYENTE_H
#define OYENTE_H

#include <iostream>

#include "common_thread.h"
#include "common_socket.h"
#include "server_oyente.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Oyente : public Thread {
private:

	Socket socket;				// Socket en el que escucha el 
								// servidor.

public:

	// Constructor
	Oyente();

	// Destructor
	~Oyente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();
};

#endif
