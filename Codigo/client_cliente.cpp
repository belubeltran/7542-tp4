//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>
#include "common_convertir.h"
#include "common_codigo_draka.h"
#include "common_comunicador.h"
#include "client_cliente.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


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
	// Creamos socket
	this->socket.crear();

	try {
		// Conectamos el socket
		this->socket.conectar(nombreHost, puerto);
	}
	catch(char const * e) {
		std::cerr << e << std::endl;
		return;
	}
	
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(&this->socket);
	
	// Enviamos petición de parte de trabajo
	if(comunicador.emitir(C_GET_JOB_PART, "") == -1) return;

	// Variables de procesamiento
	std::string instruccion;
	std::string args;

	// Recibimos respuesta del servidor
	if(comunicador.recibir(instruccion, args) == -1) return;

	// Caso en que no hay trabajo para realizar
	if(instruccion == S_NO_JOB_PART) {
		// Desconectamos el socket y salimos
		this->socket.cerrar();
		return;
	}
	else if (instruccion == S_JOB_PART) {
		// Variables auxiliares para datos
		std::string msgEncriptado, numParte;
		int numDig, claveIni, claveFin;
		std::stringstream args_stream(args);

		// Parseamos y obtenemos datos del argumento
		args_stream >> msgEncriptado >> numParte >> numDig >> claveIni 
			>> claveFin;

		// Probamos el rango de claves indicado por el servidor
		procesarClaves(msgEncriptado, numDig, claveIni, claveFin);

		// Avisamos al servidor la finalización del trabajo
		if(comunicador.emitir(C_JOB_PART_FINISHED, numParte) == -1) return;
	}
	else
		std::cerr << "Mensaje inválido del servidor" << std::endl;

	// Desconectamos el socket
	this->socket.cerrar();
}


// Prueba una a una las claves en el código Draka y envía al servidor
// aquellas claves que pasen la prueba.
void Cliente::procesarClaves(std::string msgEncriptado, int numDig, 
	int claveIni, int claveFin) {
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(&this->socket);

	// Iteramos hasta procesar todo el rango de claves
	for(int i = claveIni; i <= claveFin; i++) {
		// Convertimos clave en string
		std::string clave(Convertir::itos(i));

		// Convertimos el mensaje encriptado en el tipo necesario
		uint8_t *uintMsgEncriptado = Convertir::htoui(msgEncriptado);
		size_t len = msgEncriptado.size() / 2;

		// Probamos la clave
		if (CodigoDraka::probarClave(uintMsgEncriptado, len, clave))
			// Enviamos mensaje de posible clave si da positiva la prueba
			if(comunicador.emitir(C_POSSIBLE_KEY, clave) == -1) return;

		delete[] uintMsgEncriptado;
	}
}
