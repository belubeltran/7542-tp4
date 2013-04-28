

#include <iostream>
#include "common_socket.h"


int main(int argc, char* argv[]) {
	while(true) {
		Socket socketCLI;

		char bufin[100];
		char bufout[100];
		int numBytes;

		socketCLI.conectar(argv[1], 8000);

		std::cin >> bufin;

		numBytes = socketCLI.enviar(bufin, 100-1);
		numBytes = socketCLI.recibir(bufout, 100-1);
		bufout[numBytes] = '\0';

		std::cout << bufout << std::endl;
		socketCLI.cerrar();
	}

	return 0;
}
