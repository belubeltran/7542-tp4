//  
//  server_servidor.h
//  CLASE SERVIDOR
//  


#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include "common_thread.h"
#include "common_socket.h"
#include "common_lista.h"
#include "server_conexion_cliente.h"
#include "server_controlador_de_tareas.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Servidor : public Thread {
private:

	Lista<ConexionCliente*> *clientes;			// Lista de clientes conectados
	int puerto;									// Puerto en el que se escucha.
	Socket socket;								// Socket en el que escucha el 
												// servidor.
	ControladorDeTareas *controlador;			// Controlador de tareas.
	Lista<std::string> *claves;					// Lista de posibles claves.

	// Envía a la salida estándar la situación en la que se encuentra al
	// momento de ser invocada.
	void imprimirSituacion();					

public:

	// Constructor
	Servidor(int puerto, const  std::string& archivo, int numDigitosClave, 
		int numClientes);

	// Destructor
	~Servidor();

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clientes.
	virtual void run();

	// Detiene la ejecución del servidor
	void detener();

	void iniciar();
};

#endif
