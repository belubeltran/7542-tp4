//
//  server_asignador_tarea.h
//  CLASE ASIGNADORTAREA
//  


#ifndef ASIGNADOR_TAREA_H
#define ASIGNADOR_TAREA_H


#include "common_mutex.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class AsignadorTarea {
private:

	int& numDigitosClave;						// Número de dígitos de la
												// clave.
	int& numClientes;							// Número de clientes entre los
												// que se dividira el trabajo.
	int numPartes;								// Cantidad de partes asignadas
	std::string& msgEncriptado;					// Referencia al mensaje
	Mutex m;									// Mutex

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
	AsignadorTarea(int& numDigitosClave, int& numClientes,
		std::string& msgEncriptado);

	// Destructor
	~AsignadorTarea();

	// Genera el mensaje a ser devuelto al cliente con las indicaciones de
	// la tarea que debe realizar, si es que las hay.
	// PRE: 'msg_tarea' es una referencia a la variable en donde se depositará
	// el mensaje de indicación de tarea.
	// POST: devuelve false si no hay tareas para asignar o true si se asignó
	// tarea
	bool darIndicacion(std::string& msg_tarea);
};

#endif
