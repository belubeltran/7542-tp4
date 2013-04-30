//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  
//  
//  



#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H


#include <iostream>
#include "common_thread.h"


class ConexionCliente : public Thread {
private:

	// Servidor miServidor			//
	Socket *socket;				// Socket de transmisión de datos
	int id;

public:

	ConexionCliente() { }

	ConexionCliente(Socket *s, int id) : socket(s), id(id) { }

	~ConexionCliente() {
		this->socket->cerrar();
	}

	virtual void run() {
		
		while(true) {
			char buf[100];

			this->socket->recibir(buf, 100-1);

			if(buf[0] == 'q') {
				this->socket->cerrar();
				stop();
				std::cout << "Se desconectó cliente " << this->id << std::endl;
				break;
			}

			this->socket->enviar(buf, 100-1);
			std::cout << "Echo al cliente " << this->id << std::endl;
		}
	}
};

#endif
