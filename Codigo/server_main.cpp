//
//  server_main.cpp
//  Programa principal del SERVER
//  


#include <iostream>
#include <stdlib.h>
#include "server_servidor.h"
#include "server_terminal.h"



int main(int argc, char* argv[]) {
	// Corroboramos cantidad de argumentos
	if(argc != 5) {
		std::cout << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	// Creamos el servidor
	Servidor *servidor = new Servidor(atoi(argv[1]), argv[2], atoi(argv[3]), 
		atoi(argv[4]));

	Terminal *terminal = new Terminal(servidor);

	// Iniciamos su ejecución
	servidor->start();
	terminal->start();
	
	terminal->join();
	servidor->join();

	// delete servidor;
	delete terminal;

	return 0;
}
