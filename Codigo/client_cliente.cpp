//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include "client_cliente.h"
#include "common_convertir.h"
#include "common_codigo_draka.h"



// Constructor
Cliente::Cliente(std::string nombreHost, int puerto) : 
	puerto(puerto), nombreHost(nombreHost) { }


// Destructor
Cliente::~Cliente() {
	this->socket.cerrar();
}


// Define tareas a ejecutar en el hilo.
// Mantiene la comunicación con el servidor.
void Cliente::run() {

	try {
		this->socket.conectar(nombreHost, puerto);
		while(true);
		std::string msg("GET-JOB-PART\n");
		this->socket.enviar(msg.c_str(), msg.size()-1);
		char bufout[100];
		this->socket.recibir(bufout, 100-1);
		std::cout << bufout << std::endl;

		std::string msg1("q");
		this->socket.enviar(msg1.c_str(), msg1.size()-1);
		this->socket.cerrar();
	}
	catch(const char* e) {
		std::cout << e << std::endl;
		return;
	}


	// char bufin[100];
	// char bufout[100];
	// int numBytes;

	

	// while(true)
	// {
	// 	std::cin >> bufin;
	// 	numBytes = socketCLI.enviar(bufin, 100-1);

	// 	if(bufin[0] == 'q') break;

	// 	numBytes = socketCLI.recibir(bufout, 100-1);
	// 	bufout[numBytes] = '\0';

	// 	std::cout << bufout << std::endl;
	// }

	// socketCLI.cerrar();

	// Socket socketCLI;
}


// Prueba una a una las claves en el código Draka y envía al servidor
// aquellas claves que pasen la prueba.
void Cliente::procesarClaves(std::string& claveMin, std::string& claveMax) {

}



