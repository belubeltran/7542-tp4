//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   


#include <iostream>
#include <sstream>
#include "server_conexion_cliente.h"
#include "common_comunicador.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
ConexionCliente::ConexionCliente(Socket *s, ControladorDeTareas *controlador)
	: socket(s), controlador(controlador) { }


// Destructor
ConexionCliente::~ConexionCliente() { }


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(this->socket);

	// Variables de procesamiento
	std::string instruccion;
	std::string args;
	std::stringstream msg_in;
	std::string msg_tarea;


	// Esperamos hasta recibir el mensaje correcto
	while(instruccion != C_GET_JOB_PART)
		comunicador.recibir(instruccion, args);

	if(!this->controlador->obtenerIndicacion(msg_tarea)) {
		// No hay tarea asignada
		comunicador.emitir(msg_tarea);
		this->socket->cerrar();
		return;
	}

	// Enviamos la parte del trabajo correspondiente
	comunicador.emitir(msg_tarea);
	

	// Nos ponemos a la espera de posibles claves o de indicación de
	// finalización de tarea por parte del cliente
	while(true) {
		// Recibimos mensaje
		comunicador.recibir(instruccion, args);

		// Caso en que se recibe una posible clave
		if(instruccion == C_POSSIBLE_KEY)
			this->controlador->ingresarClave(args);
		else if (instruccion == C_JOB_PART_FINISHED) {
			this->controlador->clienteTerminoTarea();
			break;
		}
	}

	// Cerramos conexión
	this->socket->cerrar();
}
