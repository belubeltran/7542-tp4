//
//  client_main.cpp
//  Programa principal del CLIENTE
//  
//  


#include <iostream>
#include <string>
#include <sstream>
#include "client_cliente.h"
#include "common_convertir.h"



int main(int argc, char* argv[]) {
	// Corroboramos cantidad de argumentos
	if(argc != 2) {
		std::cout << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	// Datos para la conexión
	int puerto;
	std::string nombreHost;
	
	// Variables auxiliares
	std::string sPuerto;
	std::stringstream argumento(argv[1]);
	
	// Obtenemos dirección IP o nombre del host
	getline(argumento, nombreHost, ':');

	// Obtenemos el puerto 
	getline(argumento, sPuerto, ':');
	puerto = Convertir::stoi(sPuerto);

	try {
		// Creamos el cliente
		Cliente *cliente = new Cliente(nombreHost, puerto);

		// Iniciamos su ejecución
		cliente->start();
		cliente->join();

		delete cliente;
	}
	catch (const char* e) {
		std::cout << e << std::endl;
	}
	
	return 0;
}
