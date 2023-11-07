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
#include <chrono>
#include <semaphore.h>

using namespace std;

//Se crea la variable del semaforo
sem_t semaforo;

// Definiremos las funciones que se utilizaran para la sumatoria
// Función para verificar que la entrada sea un número entero
int obtenerNumeroEntero() {
    int numero;
    bool entradaValida = false;
    do {
        // Obtiene la entrada del usuario
        string entrada;
        cin >> entrada;
        entradaValida = true;
        // Verifica que cada caracter de la entrada sea un dígito
        for (char c : entrada) {
            // Si no es un dígito, se marca la entrada como inválida y se muestra un mensaje de error
            if (!isdigit(c)) {
                entradaValida = false;
                cout << "Entrada no válida. Ingrese un número entero: ";
                break;
            }
        }
        // Si la entrada es válida, se convierte a un int
        if (entradaValida) {
            //stoi convierte un string a un int
            numero = stoi(entrada);
        }
        // Se repite el ciclo hasta que la entrada sea válida
    } while (!entradaValida);
    
    return numero;
}


// función calcular suma de factoriales
// Función para calcular la sumatoria 1
long long calcularFactorial(long long n) {
    long long factorial = 1;
    for (long long i = 2; i <= n; ++i) {
        factorial *= i;
    }
    return factorial;
}

// Estructura para almacenar los parámetros de cada hilo
struct ThreadParams {
    // Inicio y fin del subrango
    long long inicio;
    long long fin;
    // Resultado parcial
    long long resultado_parcial;
};

// Función que realiza la suma de factoriales en un subrango
void* calcularSumatoriaFactoriales(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        // Calcula el factorial de cada número y lo suma
        params->resultado_parcial += calcularFactorial(i);
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de factoriales en un rango determinado por el usuario
void calcularSumatoriaFactorialesParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado1) {
    resultado1 = 0;
    // Se crean los hilos
    pthread_t threads[numHilos];
    // Se crean los parámetros para cada hilo
    struct ThreadParams params[numHilos];
    // Se calcula el tamaño de cada subrango el + 1 sirve para que no se pierda el último número
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    
    for (long long i = 0; i < numHilos; ++i) {
        // Se calcula el inicio del subrango, y el i * tamanoSubrango es para que cada hilo empiece donde terminó el anterior
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        // Si es el último hilo, el fin del subrango es el límite superior 
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        // Se crea el hilo y se verifica que se haya creado correctamente, donde se pasa la función a ejecutar y los parámetros
        if (pthread_create(&threads[i], NULL, calcularSumatoriaFactoriales, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    // Se espera a que cada hilo termine y se suma el resultado parcial de cada uno
    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado1 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}

// Función para calcular la sumatoria 2
void* calcularSumatoriaPotenciasCuadradas(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        params->resultado_parcial += i * i; // Calcula la potencia cuadrada y la suma
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de potencias cuadradas en un rango determinado por el usuario
void calcularSumatoriaPotenciasCuadradasParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado2) {
    resultado2 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaPotenciasCuadradas, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado2 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 3
void* calcularSumatoriaCubos(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        params->resultado_parcial += i * i * i; // Calcula el cubo y la suma
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de cubos en un rango determinado por el usuario
void calcularSumatoriaCubosParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado3) {
    resultado3 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaCubos, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado3 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 4
// Función que realiza la suma de números pares en un subrango
void* calcularSumatoriaPares(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (i % 2 == 0) {
            params->resultado_parcial += i; // Suma números pares
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números pares en un rango determinado por el usuario
void calcularSumatoriaParesParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado4) {
    resultado4 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaPares, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado4 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 5
void* calcularSumatoriaImpares(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (i % 2 != 0) {
            params->resultado_parcial += i; // Suma números impares
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números impares en un rango determinado por el usuario
void calcularSumatoriaImparesParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado5) {
    resultado5 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaImpares, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado5 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 6
// Función para calcular la serie de Fibonacci
long long calcularFibonacci(long long n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    long long a = 0, b = 1, c;
    long long suma = 1;

    for (long long i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }

    return b; // Devuelve el último número de Fibonacci en lugar de la suma acumulativa
}
// Función que calcula la suma de Fibonacci en un subrango
void* calcularSumatoriaFibonacciEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        params->resultado_parcial += calcularFibonacci(i);
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de Fibonacci en un rango determinado por el usuario
void calcularSumatoriaFibonacciParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado6) {
    resultado6 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaFibonacciEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado6 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 7

// Función para determinar si un número es primo
bool esPrimo(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}
// Función que calcula la suma de números primos en un subrango
void* calcularSumatoriaPrimosEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (esPrimo(i)) {
            params->resultado_parcial += i; // Suma números primos
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números primos en un rango determinado por el usuario
void calcularSumatoriaPrimosParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado7) {
    resultado7 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaPrimosEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado7 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 8
// Función que calcula la suma de números pares al cuadrado en un subrango
void* calcularSumatoriaParesAlCuadradoEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (i % 2 == 0) {
            params->resultado_parcial += i * i; // Suma el cuadrado de números pares
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números pares al cuadrado en un rango determinado por el usuario
void calcularSumatoriaParesAlCuadradoParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado8) {
    resultado8 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaParesAlCuadradoEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado8 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 9
// Función que calcula la suma de números impares al cubo en un subrango
void* calcularSumatoriaImparesAlCuboEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (i % 2 != 0) {
            params->resultado_parcial += i * i * i; // Suma el cubo de números impares
        }
    }

    pthread_exit(NULL);
}
// Función para calcular la suma de números impares al cubo en un rango determinado por el usuario
void calcularSumatoriaImparesAlCuboParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado9) {
    resultado9 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaImparesAlCuboEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado9 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 10

// Función que calcula la suma de números pares al cubo en un subrango
void* calcularSumatoriaParesAlCuboEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (i % 2 == 0) {
            params->resultado_parcial += i * i * i; // Suma el cubo de números pares
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números pares al cubo en un rango determinado por el usuario
void calcularSumatoriaParesAlCuboParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado10) {
    resultado10 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaParesAlCuboEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado10 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 11
// Función que calcula la suma de números primos al cuadrado en un subrango
void* calcularSumatoriaPrimosAlCuadradoEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        if (esPrimo(i)) {
            params->resultado_parcial += i * i; // Suma el cuadrado de números primos
        }
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de números primos al cuadrado en un rango determinado por el usuario
void calcularSumatoriaPrimosAlCuadradoParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long &resultado11) {
    resultado11 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaPrimosAlCuadradoEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado11 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
// Función para calcular la sumatoria 12

// Función que calcula la suma de una secuencia aritmética en un subrango
void* calcularSumatoriaSecuenciaAritmeticaEnRango(void* arg) {
    struct ThreadParams* params = (struct ThreadParams*)arg;
    params->resultado_parcial = 0;

    for (long long i = params->inicio; i <= params->fin; ++i) {
        params->resultado_parcial += i; // Suma de la secuencia aritmética
    }

    pthread_exit(NULL);
}

// Función para calcular la suma de una secuencia aritmética en un rango determinado por el usuario
void calcularSumatoriaSecuenciaAritmeticaParalela(long long limiteInferior, long long limiteSuperior, long long numHilos, long long & resultado12) {
    resultado12 = 0;
    pthread_t threads[numHilos];
    struct ThreadParams params[numHilos];
    long long tamanoSubrango = (limiteSuperior - limiteInferior + 1) / numHilos;

    for (long long i = 0; i < numHilos; ++i) {
        params[i].inicio = limiteInferior + i * tamanoSubrango;
        params[i].fin = (i == numHilos - 1) ? limiteSuperior : params[i].inicio + tamanoSubrango - 1;

        if (pthread_create(&threads[i], NULL, calcularSumatoriaSecuenciaAritmeticaEnRango, &params[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %lld\n", i);
            exit(1);
        }
    }

    for (long long i = 0; i < numHilos; ++i) {
        pthread_join(threads[i], NULL);
        sem_wait(&semaforo);
        resultado12 += params[i].resultado_parcial;
        sem_post(&semaforo);
    }
}
//Funciones para encontrar mayor y menor
// Funciones para encontrar el menor y el mayor de los resultados
long long encontrarMenor(long long a, long long b) {
    // Si a es menor que b, devuelve a, de lo contrario devuelve b
    return (a < b) ? a : b;
}

long long encontrarMayor(long long a, long long b) {
    // Si a es mayor que b, devuelve a, de lo contrario devuelve b
    return (a > b) ? a : b;
}


// Aquí estará el menú con el uso de pthreads
int main() {
    // Se declaran las variables necesarias
    long long limiteInferior, limiteSuperior, resultado1 = 0, resultado2 = 0, resultado3 = 0, resultado4 = 0, resultado5 = 0, resultado6 = 0, resultado7 = 0, resultado8 = 0, resultado9 = 0, resultado10 = 0, resultado11 = 0, resultado12 = 0;
    long long numHilos;

    //Se inicializa el semaforo
    sem_init(&semaforo, 0, 1);

    // Se imprimen los mensajes de bienvenida y las sumatorias a calcular
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Bienvenidos a las Operaciones matemáticas extensas." << endl;
    cout << "---------------------------------------------------" << endl;
    cout << endl;
    cout << "SUMATORIAS A CALCULAR: " << endl;
    cout << "1. Sumatoria de factoriales" << endl;
    cout << "2. Sumatoria de potencias cuadradas" << endl;
    cout << "3. Sumatoria de cubos" << endl;
    cout << "4. Sumatoria de números pares" << endl;
    cout << "5. Sumatoria de impares" << endl;
    cout << "6. Sumatoria de fibonacci" << endl;
    cout << "7. Sumatoria de números primos" << endl;
    cout << "8. Sumatoria de números pares al cuadrado" << endl;
    cout << "9. Sumatoria de números impares al cubo" << endl;
    cout << "10. Sumatoria de números pares al cubo" << endl;
    cout << "11. Sumatoria de números primos al cuadrado" << endl;
    cout << "12. Sumatoria de una secuencia aritmética" << endl;
    cout << endl;

    // Se piden los límites superior e inferior de las sumatorias

    cout << "Ingrese el límite inferior: " << endl;
    limiteInferior = obtenerNumeroEntero();
    cout << endl;

    cout << "Ingrese el límite superior: " << endl;
    limiteSuperior = obtenerNumeroEntero();
    cout << endl;

    // Se pide el número de hilos a utilizar
    cout << "Ingrese el número de hilos: ";
    numHilos = obtenerNumeroEntero();
    cout << endl;

    // Se calcula el tiempo de cada sumatoria
    auto start_time_factoriales = chrono::high_resolution_clock::now();
    calcularSumatoriaFactorialesParalela(limiteInferior, limiteSuperior, numHilos, resultado1);
    auto end_time_factoriales = chrono::high_resolution_clock::now();
    auto start_time_potencias_cuadradas = chrono::high_resolution_clock::now();
    calcularSumatoriaPotenciasCuadradasParalela(limiteInferior, limiteSuperior, numHilos, resultado2);
    auto end_time_potencias_cuadradas = chrono::high_resolution_clock::now();
    auto start_time_cubos = chrono::high_resolution_clock::now();
    calcularSumatoriaCubosParalela(limiteInferior, limiteSuperior, numHilos, resultado3);
    auto end_time_cubos = chrono::high_resolution_clock::now();
    auto start_time_pares = chrono::high_resolution_clock::now();
    calcularSumatoriaParesParalela(limiteInferior, limiteSuperior, numHilos, resultado4);
    auto end_time_pares = chrono::high_resolution_clock::now();
    auto start_time_impares = chrono::high_resolution_clock::now();
    calcularSumatoriaImparesParalela(limiteInferior, limiteSuperior, numHilos, resultado5);
    auto end_time_impares = chrono::high_resolution_clock::now();
    auto start_time_fibonacci = chrono::high_resolution_clock::now();
    calcularSumatoriaFibonacciParalela(limiteInferior, limiteSuperior, numHilos, resultado6);
    auto end_time_fibonacci = chrono::high_resolution_clock::now();
    auto start_time_primos = chrono::high_resolution_clock::now();
    calcularSumatoriaPrimosParalela(limiteInferior, limiteSuperior, numHilos, resultado7);
    auto end_time_primos = chrono::high_resolution_clock::now();
    auto start_time_pares_cuadrado = chrono::high_resolution_clock::now();
    calcularSumatoriaParesAlCuadradoParalela(limiteInferior, limiteSuperior, numHilos, resultado8);
    auto end_time_pares_cuadrado = chrono::high_resolution_clock::now();
    auto start_time_impares_cubo = chrono::high_resolution_clock::now();
    calcularSumatoriaImparesAlCuboParalela(limiteInferior, limiteSuperior, numHilos, resultado9);
    auto end_time_impares_cubo = chrono::high_resolution_clock::now();
    auto start_time_pares_cubo = chrono::high_resolution_clock::now();
    calcularSumatoriaParesAlCuboParalela(limiteInferior, limiteSuperior, numHilos, resultado10);
    auto end_time_pares_cubo = chrono::high_resolution_clock::now();
    auto start_time_primos_cuadrado = chrono::high_resolution_clock::now();
    calcularSumatoriaPrimosAlCuadradoParalela(limiteInferior, limiteSuperior, numHilos, resultado11);
    auto end_time_primos_cuadrado = chrono::high_resolution_clock::now();
    auto start_time_secuencia_aritmetica = chrono::high_resolution_clock::now();
    calcularSumatoriaSecuenciaAritmeticaParalela(limiteInferior, limiteSuperior, numHilos, resultado12);
    auto end_time_secuencia_aritmetica = chrono::high_resolution_clock::now();



    double elapsed_time_factoriales = chrono::duration_cast<chrono::nanoseconds>(end_time_factoriales - start_time_factoriales).count();
    double elapsed_time_potencias_cuadradas = chrono::duration_cast<chrono::nanoseconds>(end_time_potencias_cuadradas - start_time_potencias_cuadradas).count();
    double elapsed_time_cubos = chrono::duration_cast<chrono::nanoseconds>(end_time_cubos - start_time_cubos).count();
    double elapsed_time_pares = chrono::duration_cast<chrono::nanoseconds>(end_time_pares - start_time_pares).count();
    double elapsed_time_impares = chrono::duration_cast<chrono::nanoseconds>(end_time_impares - start_time_impares).count();
    double elapsed_time_fibonacci = chrono::duration_cast<chrono::nanoseconds>(end_time_fibonacci - start_time_fibonacci).count();
    double elapsed_time_primos = chrono::duration_cast<chrono::nanoseconds>(end_time_primos - start_time_primos).count();
    double elapsed_time_pares_cuadrado = chrono::duration_cast<chrono::nanoseconds>(end_time_pares_cuadrado - start_time_pares_cuadrado).count();
    double elapsed_time_impares_cubo = chrono::duration_cast<chrono::nanoseconds>(end_time_impares_cubo - start_time_impares_cubo).count();
    double elapsed_time_pares_cubo = chrono::duration_cast<chrono::nanoseconds>(end_time_pares_cubo - start_time_pares_cubo).count();
    double elapsed_time_primos_cuadrado = chrono::duration_cast<chrono::nanoseconds>(end_time_primos_cuadrado - start_time_primos_cuadrado).count();
    double elapsed_time_secuencia_aritmetica = chrono::duration_cast<chrono::nanoseconds>(end_time_secuencia_aritmetica - start_time_secuencia_aritmetica).count();


    cout << "La sumatoria de factoriales de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado1 << endl;
    cout << "Tiempo de cálculo de factoriales: " << elapsed_time_factoriales << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de potencias cuadradas de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado2 << endl;
    cout << "Tiempo de cálculo de potencias cuadradas: " << elapsed_time_potencias_cuadradas << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de cubos de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado3 << endl;
    cout << "Tiempo de cálculo de cubos: " << elapsed_time_cubos << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números pares de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado4 << endl;
    cout << "Tiempo de cálculo de números pares: " << elapsed_time_pares << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números impares de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado5 << endl;
    cout << "Tiempo de cálculo de números impares: " << elapsed_time_impares << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de Fibonacci de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado6 << endl;
    cout << "Tiempo de cálculo de Fibonacci: " << elapsed_time_fibonacci << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números primos de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado7 << endl;
    cout << "Tiempo de cálculo de números primos: " << elapsed_time_primos << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números pares al cuadrado de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado8 << endl;
    cout << "Tiempo de cálculo de números pares al cuadrado: " << elapsed_time_pares_cuadrado << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números impares al cubo de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado9 << endl;
    cout << "Tiempo de cálculo de números impares al cubo: " << elapsed_time_impares_cubo << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números pares al cubo de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado10 << endl;
    cout << "Tiempo de cálculo de números pares al cubo: " << elapsed_time_pares_cubo << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de números primos al cuadrado de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado11 << endl;
    cout << "Tiempo de cálculo de números primos al cuadrado: " << elapsed_time_primos_cuadrado << " nanosegundos" << endl;
    cout << endl;
    cout << "La sumatoria de una secuencia aritmética de " << limiteInferior << " a " << limiteSuperior << " es: " << resultado12 << endl;
    cout << "Tiempo de cálculo de una secuencia aritmética: " << elapsed_time_secuencia_aritmetica << " nanosegundos" << endl;
    cout << endl;

    long long sumaResultados = resultado1 + resultado2 + resultado3 + resultado4 + resultado5 +
                          resultado6 + resultado7 + resultado8 + resultado9 + resultado10 +
                          resultado11 + resultado12;

    // Calcular el promedio
    double promedio = static_cast<double>(sumaResultados) / 12.0;

    // Imprimir el promedio
    cout << "El promedio de las sumatorias es: " << promedio << endl;

    // Se calcula el mayor de las sumatorias
    long long mayor = resultado1;

    mayor = encontrarMayor(mayor, resultado2);
    mayor = encontrarMayor(mayor, resultado3);
    mayor = encontrarMayor(mayor, resultado4);
    mayor = encontrarMayor(mayor, resultado5);
    mayor = encontrarMayor(mayor, resultado6);
    mayor = encontrarMayor(mayor, resultado7);
    mayor = encontrarMayor(mayor, resultado8);
    mayor = encontrarMayor(mayor, resultado9);
    mayor = encontrarMayor(mayor, resultado10);
    mayor = encontrarMayor(mayor, resultado11);
    mayor = encontrarMayor(mayor, resultado12);

    // Se imprime el mayor de las sumatorias
    cout <<endl;
    cout << "El mayor de los resultados es: " << mayor << endl;
    cout << endl;

    // Se calcula el menor de las sumatorias
    long long menor = resultado1;

    menor = encontrarMenor(menor, resultado2);
    menor = encontrarMenor(menor, resultado3);
    menor = encontrarMenor(menor, resultado4);
    menor = encontrarMenor(menor, resultado5);
    menor = encontrarMenor(menor, resultado6);
    menor = encontrarMenor(menor, resultado7);
    menor = encontrarMenor(menor, resultado8);
    menor = encontrarMenor(menor, resultado9);
    menor = encontrarMenor(menor, resultado10);
    menor = encontrarMenor(menor, resultado11);
    menor = encontrarMenor(menor, resultado12);

    // Se imprime el menor de las sumatorias
    cout << "El menor de los resultados es: " << menor << endl;

    // Se destruye el semaforo
    sem_destroy(&semaforo);

    return 0;
}
