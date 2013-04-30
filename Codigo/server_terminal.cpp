//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  
//  
//  


#include <iostream>
#include "server_terminal.h"



// Constructor
Terminal::Terminal(Servidor *servidor) : servidor(servidor) { }


// Define tareas a ejecutar en el hilo.
// Mantiene activa la entrada estandar para recibir comandos del servidor
void Terminal::run() {
	
	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		std::string comando;
		getline(std::cin, comando);

		if(comando == "q"){
			this->servidor->apagar();
			this->stop();
		}
	}
}
