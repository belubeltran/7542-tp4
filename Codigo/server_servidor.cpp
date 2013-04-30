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

	// Creamos la lista de clientes conectados
	this->clientes = new std::list<ConexionCliente*>;

	// Creamos el asignadore de tareas para el servidor
	this->asignadorTarea = new AsignadorTarea(this->numDigitosClave, 
		this->numClientes, this->msgEncriptado);

	// Creamos el oyente para el servidor
	this->oyente = new Oyente(this->puerto, this->clientes, 
		this->cantClientesConectados, this->asignadorTarea);
}


// Destructor
Servidor::~Servidor() {
	while(!this->clientes->empty()) {
		ConexionCliente *cc = clientes->front();
		clientes->pop_front();
		delete cc;
	}

	delete this->clientes;
	delete this->oyente;
	delete this->asignadorTarea;
}


// Define tareas a ejecutar en el hilo.
// Mantiene a la escucha al servidor y acepta nuevos clients.
void Servidor::run() {
	// Iniciamos ejecucion de los recursos del servidor
	this->oyente->start();

	// Esperamos que terminen tareas
	this->oyente->join();
}


// Detiene la ejecución del servidor
void Servidor::detener() {
	this->oyente->detener();
	this->stop();
}
