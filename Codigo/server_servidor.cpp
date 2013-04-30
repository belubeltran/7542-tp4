//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  
//  
//  


#include <fstream>
#include <math.h>
#include <sstream>
#include "server_servidor.h"
#include "common_convertir.h"
#include "common_lock.h"


// Constantes
namespace {
	const int MAX_CONEXIONES = 10;
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Servidor::Servidor(int puerto, const std::string& archivo, 
	int numDigitosClave, int numClientes) : puerto(puerto), 
	numDigitosClave(numDigitosClave), numClientes(numClientes),
	cantClientesConectados(0) {
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


// Destructor
Servidor::~Servidor() {
	while(!this->clientes.empty()) {
		ConexionCliente *cc = clientes.front();
		clientes.pop_front();
		delete cc;
	}
}


// Define tareas a ejecutar en el hilo.
// Mantiene a la escucha al servidor y acepta nuevos clients.
void Servidor::run() {
	// Iniciamos la escucha del servidor
	this->socket.enlazar(this->puerto);
	this->socket.escuchar(MAX_CONEXIONES);
	// std::cout << "Servidor escuchando..." << std::endl;

	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		
		Socket *socketCLI = 0;

		//Aceptamos nuevo cliente
		try {
			socketCLI = this->socket.aceptar();
		}
		catch (const char* e) {
			// Si el socket no esta activo salteamos instrucciones
			// siguientes
			if(!this->socket.estaActivo()) break;
		}

		// Generamos una nueva conexión para este
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
			this->cantClientesConectados);

		// Censamos al cliente en el servidor
		this->clientes.push_back(conexionCLI);
		this->cantClientesConectados++;

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
	}
}


// Terminal la ejecución del servidor
void Servidor::apagar() {
	this->stop();
	this->socket.cerrar();
}


// Devuelve la clave inicial del espacio de claves a asignar a un cliente.
// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
// POST: se devuelve una cadena con la clave inicial.
std::string Servidor::claveInicialDeRangoDeClaves(int numCliente) {
	if(numCliente <= this->numClientes - 1)	{
		std::stringstream cotaInf;
		int num = numCliente*(pow(10, this->numDigitosClave) / 
			this->numClientes);

		cotaInf << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaInf.str();
	} 

	return "";
}


// Devuelve la clave final del espacio de claves a asignar a un cliente.
// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
// POST: se devuelve una cadena con la clave final.
std::string Servidor::claveFinalDeRangoDeClaves(int numCliente) {
	std::stringstream cotaSup;

	if(numCliente < this->numClientes - 1) {
		int num = (numCliente + 1)*(pow(10, this->numDigitosClave) / 
			this->numClientes) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	} 
	else if(numCliente == this->numClientes - 1) {
		int num = pow(10, this->numDigitosClave) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	}

	return "";
}
