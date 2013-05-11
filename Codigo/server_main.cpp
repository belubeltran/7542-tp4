//  
//  EL CÓDIGO DRAKA
//  Programa principal del SERVIDOR
//  
//  ***************************************************************************
//  
//  Facultad de Ingeniería - UBA
//  75.42 Taller de Programación I
//  Trabajo Práctico N°4
//  
//  ALUMNO: Federico Martín Rossi
//  PADRÓN: 92086
//  EMAIL:  federicomrossi@gmail.com
//  
//  ***************************************************************************
//
//  Programa servidor el cual se encarga de estar a la escucha de conexiones
//  entrantes por parte de clientes, y de fraccionar el trabajo a enviarles a
//  estos a medida que se van conectando. El trabajo se reparte entre un número
//  fijo de clientes especificado al iniciar el programa, provocando que, una
//  vez repartidas todas las partes, los demás clientes sean notificados de la 
//  inexistencia de parte de trabajo a asignarles.
//  
//  
//  
//  FORMA DE USO
//  ============
//
//  Deberá ejecutarse el programa en la línea de comandos de la siguiente 
//  manera:
//  
//  	# ./server [PUERTO] [ARCHIVO] [NUM-DIGITOS-CLAVE] [NUM-CLIENTES]
//
//  donde,
//
//  	PUERTO: es el servidor donde deberá escuchar el servidor;
//  	ARCHIVO: es la ruta al archivo binario conteniendo los datos
//  			 encriptados;
//  	NUM-DIGITOS-CLAVE: es el npumero de dígitos de la clave
//  	NUM-CLIENTES: es el número de clientes entre los que se dividirá el
//  	trabajo
//
//  



#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "common_convertir.h"
#include "common_mutex.h"
#include "common_lock.h"
#include "server_servidor.h"



namespace {
	// Constantes que definen los comandos válidos
	const std::string CMD_SALIR = "q";
}



/* ****************************************************************************
 * FUNCIONES AUXILIARES
 * ***************************************************************************/


// Función que se encarga de abrir un archivo, devolviendo su contenido en
// hexadecimla como un string.
// PRE: 'archivo' es la ruta del archivo, incluyendo directorio y extensión.
// POST: se devuelve un string con contenido en formato hexadecimal.
std::string abrirArchivoEncriptado(const std::string& archivo) {
	// Abrimos el archivo con el mensaje encriptado
	std::ifstream archivoMsg(archivo.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	if(!archivoMsg.is_open())
		throw "ERROR: Archivo de entrada inválido.";

	std::ifstream::pos_type size;
	uint8_t * msgTemp;

	// Almacenamos momentaneamente el mensaje original
	size = archivoMsg.tellg();
	msgTemp = new uint8_t[size];
	archivoMsg.seekg(0, std::ios::beg);
	archivoMsg.read((char*)msgTemp, size);
	archivoMsg.close();

	// Convertimos el mensaje encriptado a hexadecimal
	std::string msg_hex(Convertir::uitoh(msgTemp, size));
	delete[] msgTemp;

	return msg_hex;
}



/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


int main(int argc, char* argv[]) {
	// Corroboramos cantidad de argumentos
	if(argc != 5) {
		std::cerr << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	std::string msg;

	try {
		// Leemos archivo
		msg = abrirArchivoEncriptado(argv[2]);
	}
	catch(char const * e) {
		std::cerr << e << std::endl;
		return 1;
	}

	// Creamos el servidor
	Servidor *servidor = new Servidor(atoi(argv[1]), msg, atoi(argv[3]), 
		atoi(argv[4]));

	try {
		// Iniciamos servidor
		servidor->iniciar();
	}
	catch(char const * e) {
		std::cerr << e << std::endl;
		delete servidor;
		return 1;
	}


	std::string comando;
	
	// Esperamos a que se indique la finalización de la ejecución
	while(comando != CMD_SALIR)
		getline(std::cin, comando);


	servidor->detener();
	servidor->join();

	// Imprimimos situación del servidor luego de la ejecución del mismo
	servidor->imprimirSituacion();

	// delete terminal;
	delete servidor;

	return 0;
}
