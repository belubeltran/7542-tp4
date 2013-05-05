//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  
//  
//  

#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "server_asignador_tarea.h"
#include "common_convertir.h"
#include "common_lock.h"



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
AsignadorTarea::AsignadorTarea(int& numDigitosClave, int& numClientes,
	std::string& msgEncriptado) : numDigitosClave(numDigitosClave), 
	numClientes(numClientes), numPartes(0), msgEncriptado(msgEncriptado) { }


// Destructor
AsignadorTarea::~AsignadorTarea() { }


// Genera el mensaje a ser devuelto al cliente con las indicaciones de
// la tarea que debe realizar, si es que las hay.
// PRE: 'numCliente' es el número de cliente que solocita indicaciones.
// POST: devuelve un string con el mensaje a devolver al cliente
std::string AsignadorTarea::darIndicacion() {
	// Bloqueamos el mutex
	Lock l(m);

	// Caso en el que se han asignado ya todas las partes
	if(this->numPartes == this->numClientes - 1)
		return std::string("NO-JOB-PART\n");

	// Calculamos rango de claves
	std::string claveIni = claveInicialDeRangoDeClaves();
	std::string claveFin = claveFinalDeRangoDeClaves();
	std::string ind("");
	
	ind += "JOB-PART " + this->msgEncriptado + " " + 
		Convertir::itos(this->numPartes) + " " + 
		Convertir::itos(this->numDigitosClave) + " " + claveIni + " " +
		claveFin + "\n";

	this->numPartes++;

	return ind;
}


// Devuelve la clave inicial del espacio de claves a asignar a un cliente.
// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
// POST: se devuelve una cadena con la clave inicial.
std::string AsignadorTarea::claveInicialDeRangoDeClaves() {
	std::stringstream cotaInf;
	int num = this->numPartes*(pow(10, this->numDigitosClave) / 
		this->numClientes);

	cotaInf << std::setw(this->numDigitosClave) << std::setfill('0') 
		<< num;
	
	return cotaInf.str();
}


// Devuelve la clave final del espacio de claves a asignar a un cliente.
// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
// POST: se devuelve una cadena con la clave final.
std::string AsignadorTarea::claveFinalDeRangoDeClaves() {
	std::stringstream cotaSup;

	if(this->numPartes < this->numClientes - 1) {
		int num = (this->numPartes + 1)*(pow(10, this->numDigitosClave) / 
			this->numClientes) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	} 
	else if(this->numPartes == this->numClientes - 1) {
		int num = pow(10, this->numDigitosClave) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	}

	return "";
}
