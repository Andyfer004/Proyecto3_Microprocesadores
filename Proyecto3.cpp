/*--------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
* CC3086 Programación de microprocesadores
* Proyecto 3: Temario 7 - Operaciones matemáticas extensas
* 
* Proyecto3.cpp
* Original: Using Pthreds in C++
* Modificado por: 
* Andy Fuentes 22944
* Sergio Orellana 221122
* Rodrigo Mancilla 22611
* Carlos Valladares 22
* Fecha: 16/09/2023
* Descripción: Cálculo de 12 sumatorias con sus respectivos límites y utilizar paralelismo para obtener el resultado de las
* mismas e imprimir en pantalla el aviso de que el cálculo ha terminado. Presentar un reporte del tiempo que se necesitó para obtener cada resultado.
* --------------------------------------------------------*/


// importar librerias
#include <iostream>
#include <pthread.h>

using namespace std;

// Definiremos las funciones que se utilizaran para la sumatoria


// Function to be executed by thread
void* threadFunction(void* arg) {
    // Cast argument to int pointer
    int* numPtr = (int*) arg;
    int num = *numPtr;
    cout << "Thread " << num << " started" << endl;

    // Do some work
    for (int i = 0; i < 5; i++) {
        cout << "Thread " << num << " is working" << endl;
    }

    cout << "Thread " << num << " finished" << endl;

    // Return a value
    int* result = new int;
    *result = num * 2;
    return (void*) result;
}
// Aquí estará el menú con el uso de pthreads
int main() {
    const int NUM_THREADS = 3;
    pthread_t threads[NUM_THREADS];
    int threadArgs[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i] = i;
        int rc = pthread_create(&threads[i], NULL, threadFunction, (void*) &threadArgs[i]);
        if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    // Wait for threads to finish and collect results
    int* result;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void**) &result);
        cout << "Thread " << i << " returned " << *result << endl;
        delete result;
    }

    return 0;
}
