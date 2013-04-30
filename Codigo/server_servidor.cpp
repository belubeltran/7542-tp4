//
//  common_servidor.cpp
//  CLASE SERVIDOR
//  
//  
//  


#include <fstream>
#include "server_servidor.h"
#include "common_convertir.h"




Servidor::Servidor(int puerto, const std::string& archivo, 
	int numDigitosClave, int numClientes) : puerto(puerto), 
	numDigitosClave(numDigitosClave), numClientes(numClientes) {
	// Abrimos el archivo con el mensaje encriptado
	std::ifstream archivoMsg(archivo.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type size;
	uint8_t * msgTemp;

	// Almacenamos momentaneamente el mensaje original
	if (archivoMsg.is_open()) {
		size = archivoMsg.tellg();
		msgTemp = new uint8_t [size];
		archivoMsg.seekg (0, std::ios::beg);
		archivoMsg.read ((char*)msgTemp, size);
		archivoMsg.close();
	}

	// Convertimos el mensaje encriptado a hexadecimal y lo guardamos
	this->msgEncriptado = Convertir::uitoh(msgTemp, size);

	delete[] msgTemp;	
}

void Servidor::run() {

	int i = 1;

	// Iniciamos la escucha del servidor
	this->socket.enlazar(this->puerto);
	this->socket.escuchar(10);
	std::cout << "Servidor escuchando..." << std::endl;

	while(true) {
		Socket *socketCLI = this->socket.aceptar();
		// std::cout << "Se conectó el cliente " << this->contIdCliente << std::endl;
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI, i);
		i++;
		this->clientes.push_back(conexionCLI);
		conexionCLI->start();
	}

	// std::list<ConexionCliente*>::iterator i;
	// for(i = this->clientes->begin(); i != this->clientes->end(); ++i)
	// 	(*i)->join();
}

