//
//  server_terminal.h
//  CLASE TERMINAL
//  
//  
//  


#ifndef TERMINAL_H
#define TERMINAL_H


#include "common_thread.h"
#include "server_servidor.h"



class Terminal : public Thread {
private:

	Servidor *servidor;			// Puntero al servidor con el que está
								// relacionado 	

public:

	// Constructor
	Terminal(Servidor *servidor);

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clients.
	virtual void run();
};

#endif
