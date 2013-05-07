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
		std::cerr << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	// Creamos el servidor
	Servidor *servidor = new Servidor(atoi(argv[1]), argv[2], atoi(argv[3]), 
		atoi(argv[4]));

	Terminal *terminal = new Terminal(servidor);

	// Iniciamos servidor
	servidor->iniciar();

	// Iniciamos terminal de comandos
	terminal->start();

	// Esperamos a que el servidor concluya su actividad
	servidor->esperar();
	servidor->detener();

	// Detenido el servidor interrumpimos la ejecución del terminal
	terminal->cancel();
	terminal->join();

	// Imprimimos situación del servidor luego de la ejecución del mismo
	servidor->imprimirSituacion();

	delete terminal;
	delete servidor;

	return 0;
}
