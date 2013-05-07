//
//  server_main.cpp
//  Programa principal del SERVER
//  


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "server_servidor.h"
#include "server_terminal.h"
#include "common_convertir.h"




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

	Terminal *terminal = new Terminal(servidor);

	try {
		// Iniciamos servidor
		servidor->iniciar();
	}
	catch(char const * e) {
		std::cerr << e << std::endl;

		delete terminal;
		delete servidor;

		return 1;
	}

	// Iniciamos terminal de comandos
	terminal->start();

	// Esperamos a que el servidor concluya su actividad
	servidor->esperar();
	servidor->detener();

	// Detenido el servidor interrumpimos la ejecución del terminal
	terminal->cancel();
	terminal->join();

	// Imprimimos situación del servidor luego de la ejecución del mismo
	servidor->imprimirSituacion();

	delete terminal;
	delete servidor;

	return 0;
}
