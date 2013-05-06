//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   


#include <iostream>
#include <sstream>
#include "server_conexion_cliente.h"
#include "common_protocolo.h"


namespace {
	// Constante para el buffer
	const int BUFFER_TAMANIO = 100;
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
ConexionCliente::ConexionCliente(Socket *s, Lista<ConexionCliente*> *clientes,
	Lista<std::string> *claves, AsignadorTarea *asignadorTarea) : id(-1), 
	socket(s), clientes(clientes), claves(claves), 
	asignadorTarea(asignadorTarea) { }


// Destructor
ConexionCliente::~ConexionCliente() { }


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	// Variables de procesamiento
	std::string instruccion;
	std::stringstream msg_in;

	// Esperamos hasta recibir el mensaje correcto
	while(instruccion != C_GET_JOB_PART) {
		// Esperamos por el mensaje
		msg_in << recibirMensaje();

		// Parseamos instrucción recibida
		msg_in >> instruccion;
	}

	std::string msg_tarea;

	if(!this->asignadorTarea->darIndicacion(msg_tarea)) {
		this->socket->enviar(msg_tarea.c_str(), msg_tarea.size());
		this->socket->cerrar();
		// Nos damos de baja en la lista de conexiones de clientes
		this->clientes->eliminar(this);
		return;
	}

	// Enviamos la parte del trabajo correspondiente
	this->socket->enviar(msg_tarea.c_str(), msg_tarea.size());
	

	// Nos ponemos a la espera de posibles claves o de indicación de
	// finalización de tarea por parte del cliente
	while(msg_in << recibirMensaje()) {
		// Parseamos instrucción recibida
		msg_in >> instruccion;

		// Caso en que se recibe una posible clave
		if(instruccion == C_POSSIBLE_KEY) {
			// Parseamos clave
			std::string nuevaClave;
			msg_in >> nuevaClave;
			// Insertamos la clave en la lista de claves del servidor	
			this->claves->insertarUltimo(nuevaClave);
			std::cout << msg_in.str() << std::endl;
		}
		// Caso en que el cliente ha finalizado la tarea
		else if (instruccion == C_JOB_PART_FINISHED) {
			break;
		}
	}

	std::cout << "Cliente " << this->id << " finalizó la tarea" << std::endl;

	// Cerramos conexión
	this->socket->cerrar();
	// Nos damos de baja en la lista de conexiones de clientes
	this->clientes->eliminar(this);

	std::cout << "Clientes: " << this->clientes->tamanio() << std::endl;
}


// Asigna un número de identificación a la conexión
void ConexionCliente::asignarID(int id) {
	this->id = id;
}


// Devuelve la identificación de la conexión. Si no posee identificación,
// devuelve -1.
int ConexionCliente::ID() {
	return this->id = id;
}


// Recibe un mensaje entrante
// POST: devuelve un string con el mensaje recibido
std::string ConexionCliente::recibirMensaje() {
	// Variables auxiliares
	char bufout[BUFFER_TAMANIO + 1];
	std::string msg_in;
	bool estaRecibiendo = true;

	// Iteramos hasta recibir el mensaje completo
	while(estaRecibiendo) {
		bufout[BUFFER_TAMANIO] = '\0';
		
		// Recibimos datos. En caso de error devolvemos cadena vacía
		if(this->socket->recibir(bufout, BUFFER_TAMANIO) == -1) return "";

		// Agregamos datos a los datos ya recibidos
		std::string sbufout(bufout);
		msg_in.append(sbufout);

		// Comprobamos si hemos recibido el mensaje completo
		if(bufout[BUFFER_TAMANIO-1] == FIN_MENSAJE) break;
		// Si el buffer no se encuentra lleno, buscamos el fin de mensaje
		else if (!bufout[BUFFER_TAMANIO-1]) {
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
