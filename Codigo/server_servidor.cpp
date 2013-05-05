//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  


#include <fstream>
#include <math.h>
#include <sstream>
#include "server_servidor.h"
#include "common_convertir.h"



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

	// Creamos la lista de clientes conectados
	this->clientes = new std::list<ConexionCliente*>;

	// Creamos el asignadore de tareas para el servidor
	this->asignadorTarea = new AsignadorTarea(this->numDigitosClave, 
		this->numClientes, this->msgEncriptado);
}


// Destructor
Servidor::~Servidor() {

	// Liberamos espacio utilizado por cada conexión cliente
	while(!this->clientes->empty()) {
		ConexionCliente *cc = clientes->front();
		clientes->pop_front();
		delete cc;
	}

	// Liberamos espacio utilizado por atributos
	delete this->clientes;
	delete this->asignadorTarea;
}


// Define tareas a ejecutar en el hilo.
// Mantiene a la escucha al servidor y acepta nuevos clients.
void Servidor::run() {
	// Iniciamos la escucha del servidor
	this->socket.crear();
	this->socket.enlazar(this->puerto);
	this->socket.escuchar(MAX_CONEXIONES);
	std::cout << "Servidor escuchando..." << std::endl;

	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		
		Socket *socketCLI = 0;
		
		//Aceptamos nuevo cliente
		socketCLI = this->socket.aceptar();
		
		// Salimos si el socket no esta activo o si se interrumpió
		// la escucha de solicitudes de conexión
		if(!this->socket.estaActivo() || !socketCLI) break;
		
		// Generamos una nueva conexión para escuchate
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
			this->cantClientesConectados, this->asignadorTarea);

		// Censamos al cliente en el servidor
		this->clientes->push_back(conexionCLI);
		this->cantClientesConectados++;

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();

		// DEBUG CODE
		std::cout << "Hay " << this->cantClientesConectados << " ahora conectados." << std::endl;
		// END DEBUG CODE
	}
}


// Detiene la ejecución del servidor
void Servidor::detener() {
	this->stop();

	// Forzamos el cierre del socket
	this->socket.cerrar();
}
