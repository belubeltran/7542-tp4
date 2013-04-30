

#include <iostream>
#include "common_socket.h"
#include "common_codigo_draka.h"


int main(int argc, char* argv[]) {
	
	Socket socketCLI;

	char bufin[100];
	char bufout[100];
	int numBytes;

	socketCLI.conectar(argv[1], 8000);

	while(true)
	{
		std::cin >> bufin;
		numBytes = socketCLI.enviar(bufin, 100-1);

		if(bufin[0] == 'q') break;

		numBytes = socketCLI.recibir(bufout, 100-1);
		bufout[numBytes] = '\0';

		std::cout << bufout << std::endl;
	}

	socketCLI.cerrar();
	
	return 0;
}
