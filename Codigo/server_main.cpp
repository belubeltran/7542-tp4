
#include <iostream>
#include <stdlib.h>
#include "server_servidor.h"



int main(int argc, char* argv[]) {

	// Corroboramos cantidad de argumentos
	if(argc != 5) {
		std::cout << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	Servidor *serv = new Servidor(atoi(argv[1]), argv[2], atoi(argv[3]), 
		atoi(argv[4]));

	serv->start();
	serv->join();

	delete serv;

	return 0;
}
