//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <sstream>
#include "client_cliente.h"
#include "common_convertir.h"
#include "common_codigo_draka.h"


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto) : 
	puerto(puerto), nombreHost(nombreHost) { }


// Destructor
Cliente::~Cliente() { }


// Define tareas a ejecutar en el hilo.
// Mantiene la comunicación con el servidor.
void Cliente::run() {

	this->socket.crear();
	this->socket.conectar(nombreHost, puerto);
	
	// Enviamos petición de obtención de parte de trabajo
	std::string msg_out("GET-JOB-PART\n");
	int n = this->socket.enviar_todo(msg_out.c_str(), msg_out.size());
	std::cout << "Envio: " << n << std::endl;


	// Recibimos respuesta del servidor
	std::stringstream msg_in(recibirMensaje());
	// std::cout << msg_in << std::endl;

	// Parseamos instrucción recibida
	std::string instruccion;
	msg_in >> instruccion;

	// Caso en que no hay trabajo para realizar
	if(instruccion == "NO-JOB-PART") {
		// Desconectamos el socket y salimos
		std::cout << "NO-JOB-PART" << std::endl;
	}
	else if(instruccion == "JOB-PART") {
		std::cout << "A procesar se ha dicho..." << std::endl;
	}
	else if(instruccion != "JOB-PART") {
		std::cout << "Mensaje inválido del servidor" << std::endl;
	}

	// Desconectamos el socket
	this->socket.cerrar();

	return;
}


// Recibe un mensaje entrante
// POST: devuelve un string con el mensaje recibido
std::string Cliente::recibirMensaje() {
	// Variables auxiliares
	char bufout[100];
	std::string msg_in;
	bool estaRecibiendo = true;

	// Iteramos hasta recibir el mensaje completo
	while(estaRecibiendo) {
		bufout[99] = '\0';
		
		this->socket.recibir(bufout, 100-1);
		
		std::string sbufout(bufout);
		msg_in.append(sbufout);

		// std::cout << n << " - " << bufout << std::endl;
		// std::cout << "size: " << sizeof(bufout) << std::endl;

		if(bufout[98] == '\n') break;
		else if(!bufout[98])
		{
			for(int i = 0; i <= 98; i++)
				if(bufout[i] == '\n') {
					// Se recibió el marcador de fin de mensaje
					estaRecibiendo = false;
					break;
				}
		}
	}

	return msg_in;
}


// Prueba una a una las claves en el código Draka y envía al servidor
// aquellas claves que pasen la prueba.
void Cliente::procesarClaves(std::string& claveMin, std::string& claveMax) {

}



