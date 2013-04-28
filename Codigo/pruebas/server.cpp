
#include <iostream>
#include "common_socket.h"



int main(int argc, char* argv[]) {
	std::cout << "Creando socket..." << std::endl;

	Socket socket;

	std::cout << "Enlazando..." << std::endl;
	socket.enlazar(8000);
	socket.escuchar(5);
	std::cout << "Escuchando..." << std::endl << std::endl;

	while(true) {
		Socket *socketCLI = socket.aceptar();
		char buf[100];

		socketCLI->recibir(buf, 100-1);
		std::cout << "Recibí un mensaje" << std::endl;
		socketCLI->enviar(buf, 100-1);

		socketCLI->cerrar();
	}

	return 0;
}
