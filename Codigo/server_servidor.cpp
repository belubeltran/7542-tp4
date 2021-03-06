//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  


#include <iostream>
#include "server_servidor.h"
#include "common_lock.h"



// Constantes
namespace {
	const int MAX_CONEXIONES = 10;
}



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Servidor::Servidor(int puerto, std::string& msg, 
	int numDigitosClave, int numClientes) : puerto(puerto) {
	// Creamos la lista de clientes conectados
	this->clientes = new Lista<ConexionCliente*>;

	// Creamos la lista de posibles claves
	this->claves = new Lista<std::string>;

	// Creamos el controlador de tareas para el servidor
	this->controlador = new ControladorDeTareas(numDigitosClave, 
		numClientes, msg, this->claves);
}


// Destructor
Servidor::~Servidor() {
	// Concluimos la conexión con clientes existentes
	this->cerrarConexionesConClientes();

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
		// Detenemos servidor de inmediato
		this->detener();
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
// para iniciar. En caso de error lanza una excepción.
void Servidor::iniciar() {
	// Iniciamos hilo de ejecución
	this->start();
}


// Detiene la ejecución del servidor. No debe utilizarse el método stop()
// para detener.
void Servidor::detener() {
	// Detenemos hilo
	this->stop();

	// Forzamos el cierre del socket para evitar nuevas conexiones entrantes
	try {
		this->socket.cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { }

	// Concluimos la conexión con clientes existentes
	this->cerrarConexionesConClientes();
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


// Cierra todas las conexiones existentes con clientes y elimina todo 
// registro de estos,quedando vacía la lista de clientes.
void Servidor::cerrarConexionesConClientes() {
	// Detenemos y liberamos espacio utilizado por cada conexión cliente,
	// dejando vacía la lista de clientes activos.
	while(!this->clientes->estaVacia()) {
		// Obtenemos cliente y lo eliminamos de la lista
		ConexionCliente *cc = this->clientes->verPrimero();
		this->clientes->eliminarPrimero();
		
		// Detenemos conexión con el cliente
		cc->detener();
		// Esperamos a que finalice
		cc->join();
		// Liberamos memoria
		delete cc;
	}
}
