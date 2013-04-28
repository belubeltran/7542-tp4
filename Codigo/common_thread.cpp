//  
//  common_thread.cpp
//  CLASE THREAD
//  
//  Clase que implementa la interfaz para la creación de un hilo de ejecución.
//  


#include "common_thread.h"



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Thread::Thread() : status(true) { }


// Constructor privado
Thread::Thread(const Thread &c) { }


// Destructor
Thread::~Thread() { }


// Inicia el thread
void Thread::start() {
	pthread_create(&this->thread, 0, callback, this);
}


// Detiene el thread
void Thread::stop() {
	this->status = false;
}


// Bloquea hasta que el hilo finalice su ejecución en caso de estar
// ejecutandose.
void Thread::join() {
	pthread_join(this->thread, 0);
}


// Verifica si el hilo se encuentra activo.
// POST: devuelve true si está activo o false en caso contrario.
bool Thread::isActive() {
	return this->status;
}


// Ejecuta el método run().
// PRE: 'threadID' es un puntero al thread.
void* Thread::callback(void *threadID) {
	((Thread*)threadID)->run();
	return 0;
}
