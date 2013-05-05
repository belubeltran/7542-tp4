//  
//  common_codigo_draka.h
//  LIBRERIA CODIGODRAKA
//  
//  Librería de funciones relacionadas al código Draka.
//  


#include "common_codigo_draka.h"



// Código utilizado por los Draka para encriptar y desencriptar mensajes.
void CodigoDraka::ed(uint8_t *data, size_t ndata, const uint8_t *key,size_t nkey) {
	uint8_t i = 0, j = 0, s[256];
	do{ 
		s[i] = i; 
	} while (++i);

	do {
		j += s[i] + key[i % nkey];
		i - j && (s[i] ^= s[j], s[j] ^= s[i], s[i] ^= s[j]);
	} while(++i);
	
	j = 0;
	
	while(ndata--) {
		i++; j += s[i];
		i - j && (s[i] ^= s[j], s[j] ^= s[i], s[i] ^= s[j]);
		*data++ ^= s[(s[i] + s[j]) & 0xff];
	}
}


// Función que prueba una clave en el código Draka y corrobora si, al 
// desencriptar el mensaje encriptado, da como resultado texto ASCII válido
// PRE: 'dato' es un puntero al mensaje encriptado; 'lenDato' es el
// tamanio en bytes de 'dato'; 'clave' es la clave a probar.
// POST: devuelve true si la prueba da como resultado texto ASCII válido o
// false en su defecto.
bool CodigoDraka::probarClave(uint8_t *dato, size_t lenDato, const std::string& clave) {
	size_t lenClave = clave.size();

	CodigoDraka::ed(dato, lenDato, (const uint8_t *)(clave.c_str()), lenClave);

	for(size_t i = 0; i < lenDato; i++)
		if(dato[i] > 127)
			return false;

	return true;
}
