//
//  server_controlador_de_tareas.h
//  CLASE CONTROLADORDETAREAS
//  


#include <iomanip>
#include <math.h>
#include <sstream>
#include "server_controlador_de_tareas.h"
#include "common_convertir.h"
#include "common_lock.h"
#include "common_protocolo.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ControladorDeTareas::ControladorDeTareas(int numDigitosClave, int numClientes,
		std::string msgEncriptado, Lista<std::string> *claves) : 
		numDigitosClave(numDigitosClave), numClientes(numClientes), 
		msgEncriptado(msgEncriptado), claves(claves), numPartes(0), 
		asignacionCompleta(false), clientesCorriendo(0) { }


// Destructor
ControladorDeTareas::~ControladorDeTareas() { }


// Genera el mensaje a ser devuelto al cliente con las indicaciones de
// la tarea que debe realizar, si es que las hay.
// PRE: 'msg_tarea' es una referencia a la variable en donde se depositará
// el mensaje de indicación de tarea.
// POST: devuelve false si no hay tareas para asignar o true si se asignó
// tarea
bool ControladorDeTareas::obtenerIndicacion(std::string& msg_tarea) {
	// Bloqueamos el mutex
	Lock l(this->m);

	// Caso en el que se han asignado ya todas las partes
	if(asignacionCompleta) {
		msg_tarea = S_NO_JOB_PART;
		return false;
	};

	// Calculamos rango de claves
	std::string claveIni = claveInicialDeRangoDeClaves();
	std::string claveFin = claveFinalDeRangoDeClaves();
	std::string ind("");
	
	// Generamos el mensaje
	ind += S_JOB_PART + " " + this->msgEncriptado + " " + 
		Convertir::itos(this->numPartes) + " " + 
		Convertir::itos(this->numDigitosClave) + " " + claveIni + " " +
		claveFin;

	this->numPartes++;
	msg_tarea = ind;

	// Si esta fue la última parte que se debia asignar, seteamos en
	// true el flag de asignaciones completadas
	if(this->numPartes == this->numClientes)
		this->asignacionCompleta = true;

	this->clientesCorriendo++;

	return true;
}


// Recibe una posible clave para almacenar en el servidor
void ControladorDeTareas::ingresarClave(std::string clave) {
	// Bloqueamos el mutex
	Lock l(this->m);
	this->claves->insertarUltimo(clave);
}


// Permite a un cliente notificar que ha finalizado su tarea
void ControladorDeTareas::clienteTerminoTarea() {
	// Bloqueamos el mutex
	Lock l(this->m);

	// Decrementamos una unidad la cantidad de clientes corriendo
	this->clientesCorriendo--;

	// Si se terminaron de procesar todas las partes, enviamos una señal
	if(this->asignacionCompleta && (clientesCorriendo == 0))
		l.signal();
}


// Método que al ser invocado se bloquea hasta que se hayan finalizado
// los procesamientos de las partes correspondientes a las tareas.
void ControladorDeTareas::esperarTerminarTareas() {
	// Bloqueamos el mutex
	Lock l(this->m);

	// Nos ponemos a la espera de que se finalicen las tareas habiéndose
	// asignado todas las partes a procesar
	if(!this->asignacionCompleta || (this->clientesCorriendo > 0))
		l.wait();
}


// Detiene todas las tareas
void ControladorDeTareas::detenerTareas() {
	Lock l(this->m);
	// Destrabamos la espera de la finalización de tareas
	l.signal();
}


// Corrobora si se han terminado las tareas.
// POST: devuelve true si se completaron o false en su defecto.
bool ControladorDeTareas::seCompletaronTareas() {
	return (this->asignacionCompleta && (clientesCorriendo == 0));
}


// Devuelve la clave inicial del espacio de claves a asignar a un cliente.
// PRE: 'numCliente' es el número de cliente que le ha sido asignado.
// POST: se devuelve una cadena con la clave inicial.
std::string ControladorDeTareas::claveInicialDeRangoDeClaves() {
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
std::string ControladorDeTareas::claveFinalDeRangoDeClaves() {
	std::stringstream cotaSup;

	if(this->numPartes < this->numClientes - 1) {
		int num = (this->numPartes + 1)*(pow(10, this->numDigitosClave) / 
			this->numClientes) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	} 
	else if (this->numPartes == this->numClientes - 1) {
		int num = pow(10, this->numDigitosClave) - 1;

		cotaSup << std::setw(this->numDigitosClave) << std::setfill('0') 
			<< num;
		
		return cotaSup.str();
	}

	return "";
}
