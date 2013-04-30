//  
//  common_codigo_draka.h
//  LIBRERIA CODIGODRAKA
//  
//  Librería de funciones relacionadas al código Draka.
//  


#ifndef CODIGO_DRAKA_H
#define CODIGO_DRAKA_H


#include <string>
#include <stdint.h>



class CodigoDraka {
public:

	// Código utilizado por los Draka para encriptar y desencriptar mensajes.
	static void ed(uint8_t *data, size_t ndata, const uint8_t *key, 
		size_t nkey);

	// Función que prueba una clave en el código Draka y corrobora si, al 
	// desencriptar el mensaje encriptado, da como resultado texto ASCII válido
	// PRE: 'dato' es un puntero al mensaje encriptado; 'lenDato' es el
	// tamanio en bytes de 'dato'; 'clave' es la clave a probar.
	// POST: devuelve true si la prueba da como resultado texto ASCII válido o
	// false en su defecto.
	static bool probarClave(uint8_t *dato, size_t lenDato, 
		const std::string& clave);
};


#endif
