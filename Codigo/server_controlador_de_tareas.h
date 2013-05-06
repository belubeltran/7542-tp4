//
//  server_controlador_de_tareas.h
//  CLASE CONTROLADORDETAREAS
//  


#ifndef CONTROLADOR_DE_TAREAS_H
#define CONTROLADOR_DE_TAREAS_H


#include "common_mutex.h"
#include "common_lista.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ControladorDeTareas {
private:

	int numDigitosClave;						// Número de dígitos de la
												// clave.
	int numClientes;							// Número de clientes entre los
												// que se dividirá el trabajo.
	std::string msgEncriptado;					// Referencia al mensaje
	Lista<std::string> *claves;					// Lista de posibles claves.
	Mutex m;									// Mutex
	int numPartes;								// Cantidad de partes asignadas
	bool asignacionCompleta;					// Flag que contiene info sobre
												// si se asigno todo el trabajo
	int cantAsignacionesHechas;					// Número de asignaciones
												// realizadas
	int clientesCorriendo;						// Contador de los clientes que
												// se encuentran procesando

	// Devuelve la clave inicial del espacio de claves a asignar a un cliente.
	// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
	// POST: se devuelve una cadena con la clave inicial.
	std::string claveInicialDeRangoDeClaves();

	// Devuelve la clave final del espacio de claves a asignar a un cliente.
	// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
	// POST: se devuelve una cadena con la clave final.
	std::string claveFinalDeRangoDeClaves();


public:

	// Constructor
	ControladorDeTareas(int numDigitosClave, int numClientes,
		std::string msgEncriptado, Lista<std::string> *claves);

	// Destructor
	~ControladorDeTareas();

	// Genera el mensaje a ser devuelto al cliente con las indicaciones de
	// la tarea que debe realizar, si es que las hay.
	// PRE: 'msg_tarea' es una referencia a la variable en donde se depositará
	// el mensaje de indicación de tarea.
	// POST: devuelve false si no hay tareas para asignar o true si se asignó
	// tarea
	bool obtenerIndicacion(std::string& msg_tarea);

	// Recibe una posible clave para almacenar en el servidor
	void ingresarClave(std::string clave);

	// Permite a un cliente notificar que ha finalizado su tarea
	void clienteTerminoTarea();

	// Método que al ser invocado se bloquea hasta que se hayan finalizado
	// los procesamientos de las partes correspondientes a las tareas.
	void esperarTerminarTareas();

	// Detiene todas las tareas
	void detenerTareas();

	// Corrobora si se han terminado las tareas.
	// POST: devuelve true si se completaron o false en su defecto.
	bool seCompletaronTareas();
};

#endif
