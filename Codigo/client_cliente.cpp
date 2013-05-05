//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>
#include "client_cliente.h"
#include "common_convertir.h"
#include "common_codigo_draka.h"
#include "common_protocolo.h"


namespace {
	// Constante para el buffer
	const int BUFFER_TAMANIO = 100;
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto) : 
	puerto(puerto), nombreHost(nombreHost) { }


// Destructor
Cliente::~Cliente() { }


// Define tareas a ejecutar en el hilo.
// Mantiene la comunicación con el servidor.
void Cliente::run() {
	// Creamos socket
	this->socket.crear();
	this->socket.conectar(nombreHost, puerto);
	
	// Enviamos petición de parte de trabajo
	std::string msg_out(C_GET_JOB_PART + FIN_MENSAJE);
	this->socket.enviar(msg_out.c_str(), msg_out.size());

	// Recibimos respuesta del servidor
	std::stringstream msg_in(recibirMensaje());

	// Parseamos instrucción recibida
	std::string instruccion;
	msg_in >> instruccion;


	// Caso en que no hay trabajo para realizar
	if(instruccion == S_NO_JOB_PART) {
		// Desconectamos el socket y salimos
		std::cout << S_NO_JOB_PART << std::endl;
	}
	else if(instruccion == S_JOB_PART) {
		// Variables auxiliares para datos
		std::string msgEncriptado, numParte;
		int numDig, claveIni, claveFin;

		// Parseamos y obtenemos datos del mensaje
		msg_in >> msgEncriptado >> numParte >> numDig >> claveIni >> claveFin;

		// Probamos el rango de claves indicado por el servidor
		procesarClaves(msgEncriptado, numDig, claveIni, claveFin);

		// Avisamos al servidor la finalización del trabajo
		msg_out = C_JOB_PART_FINISHED + " " + numParte;
		this->socket.enviar(msg_out.c_str(), msg_out.size());
	}
	else {
		std::cout << "Mensaje inválido del servidor" << std::endl;
	}

	// Desconectamos el socket
	this->socket.cerrar();
}


// Recibe un mensaje entrante
// POST: devuelve un string con el mensaje recibido
std::string Cliente::recibirMensaje() {
	// Variables auxiliares
	char bufout[BUFFER_TAMANIO+1];
	std::string msg_in;
	bool estaRecibiendo = true;

	// Iteramos hasta recibir el mensaje completo
	while(estaRecibiendo) {
		bufout[BUFFER_TAMANIO] = '\0';
		
		// Recibimos datos. En caso de error devolvemos cadena vacía
		if(this->socket.recibir(bufout, BUFFER_TAMANIO) == -1) return "";

		// Agregamos datos a los datos ya recibidos
		std::string sbufout(bufout);
		msg_in.append(sbufout);

		// Comprobamos si hemos recibido el mensaje completo
		if(bufout[BUFFER_TAMANIO-1] == FIN_MENSAJE) break;
		// Si el buffer no se encuentra lleno, buscamos el fin de mensaje
		else if(!bufout[BUFFER_TAMANIO-1]) {
			for(int i = 0; i <= BUFFER_TAMANIO-1; i++) {
				if(bufout[i] == FIN_MENSAJE) {
					// Se recibió el marcador de fin de mensaje
					estaRecibiendo = false;
					break;
				}
			}
		}
	}

	return msg_in;
}


// Prueba una a una las claves en el código Draka y envía al servidor
// aquellas claves que pasen la prueba.
void Cliente::procesarClaves(std::string msgEncriptado, int numDig, 
	int claveIni, int claveFin) {
	// Iteramos hasta procesar todo el rango de claves
	for(int i = claveIni; i <= claveFin; i++) {
		// Convertimos clave en string
		std::string clave(Convertir::itos(i));

		// Convertimos el mensaje encriptado en el tipo necesario
		uint8_t *uintMsgEncriptado = Convertir::htoui(msgEncriptado);
		size_t len = msgEncriptado.size() / 2;

		// Probamos la clave
		if (CodigoDraka::probarClave(uintMsgEncriptado, len, clave)) {
			// Armamos mensaje de acuerdo al protocolo
			std::string msg_out(C_POSSIBLE_KEY + " " + clave);
			
			// Enviamos mensaje de posible clave
			this->socket.enviar(msg_out.c_str(), msg_out.size());
		}
	}
}
