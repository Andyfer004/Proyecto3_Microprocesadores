#include <iostream>
#include <pthread.h>

using namespace std;

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
