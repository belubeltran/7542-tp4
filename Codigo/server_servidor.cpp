//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  


#include <iostream>
#include <fstream>
#include "common_convertir.h"
#include "server_servidor.h"



// Constantes
namespace {
	const int MAX_CONEXIONES = 10;
}



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Servidor::Servidor(int puerto, const std::string& archivo, 
	int numDigitosClave, int numClientes) : puerto(puerto) {
	// Abrimos el archivo con el mensaje encriptado
	std::ifstream archivoMsg(archivo.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type size;
	uint8_t * msgTemp;

	// Almacenamos momentaneamente el mensaje original
	if (archivoMsg.is_open()) {
		size = archivoMsg.tellg();
		msgTemp = new uint8_t[size];
		archivoMsg.seekg(0, std::ios::beg);
		archivoMsg.read((char*)msgTemp, size);
		archivoMsg.close();
	}

	// Convertimos el mensaje encriptado a hexadecimal
	std::string msg_hex(Convertir::uitoh(msgTemp, size));
	delete[] msgTemp;
	
	// Creamos la lista de clientes conectados
	this->clientes = new Lista<ConexionCliente*>;

	// Creamos la lista de posibles claves
	this->claves = new Lista<std::string>;

	// Creamos el controlador de tareas para el servidor
	this->controlador = new ControladorDeTareas(numDigitosClave, 
		numClientes, msg_hex, this->claves);
}


// Destructor
Servidor::~Servidor() {
	// Liberamos espacio utilizado por cada conexión cliente
	while(!this->clientes->estaVacia()) {
		// Obtenemos cliente y lo eliminamos de la lista
		ConexionCliente *cc = this->clientes->verPrimero();
		this->clientes->eliminarPrimero();
		
		// Cancelamos la ejecución del hilo
		cc->cancel();
		// Esperamos a que finalice
		cc->join();
		// Liberamos memoria
		delete cc;
	}

	// Liberamos espacio utilizado por atributos
	delete this->clientes;
	delete this->claves;
	delete this->controlador;
}


// Define tareas a ejecutar en el hilo.
// Mantiene a la escucha al servidor y acepta nuevos clientes.
void Servidor::run() {
	try {
		// Iniciamos la escucha del servidor
		this->socket.crear();
		this->socket.escuchar(MAX_CONEXIONES, this->puerto);
	}
	catch(char const * e) {
		// Mensaje de error
		std::cerr << e << std::endl;
		// Detenemos tareas y retornamos en caso de error
		this->controlador->detenerTareas();
		return;
	}
	
	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		Socket *socketCLI = 0;

		// Aceptamos nuevo cliente
		socketCLI = this->socket.aceptar();
		
		// Salimos si el socket no esta activo o si se interrumpió
		// la escucha de solicitudes de conexión
		if(!this->socket.estaActivo() || !socketCLI) break;
		
		// Generamos una nueva conexión para escuchate
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
			this->controlador);

		// Censamos al cliente en el servidor
		this->clientes->insertarUltimo(conexionCLI);

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
	}
}


// Inicia la ejecución del servidor. No debe utilizarse el método start()
// para iniciar. 
void Servidor::iniciar() {
	this->start();
}


// Espera hasta que se termine de ejecutar el servidor de forna natural.
void Servidor::esperar() {
	// Bloquea hasta que se terminen las tareas
	this->controlador->esperarTerminarTareas();
}


// Detiene la ejecución del servidor. No debe utilizarse el método stop()
// para detener.
void Servidor::detener() {
	// Detenemos hilo
	this->stop();

	// Detenemos tareas en curso
	this->controlador->detenerTareas();
	
	// Forzamos el cierre del socket
	try {
		this->socket.cerrar();
	}
	// Ante una eventual detención abrupta, posterior a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch (...) { }

	// Esperamos a que el thread finalice su ejecución
	this->join();
}


// Envía a la salida estándar la situación en la que se encuentra al
// momento de ser invocada.
void Servidor::imprimirSituacion() {
	// Caso en que no se completaron las tareas
	if(!this->controlador->seCompletaronTareas())
		std::cout << "Not finished" << std::endl;
	// Caso en que se completaron las tareas
	else {
		// No se encontraron claves
		if(this->claves->tamanio() == 0)
			std::cout << "No keys found" << std::endl;
		// Se encontró una única clave
		else if (this->claves->tamanio() == 1)
			std::cout << this->claves->verPrimero() << std::endl;
		// Se encontraron múltiples claves
		else
			std::cout << "Multiple keys found" << std::endl;
	}
}
