#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void openmp_hello(void);

int main(int argc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);

    #pragma omp parallel num_threads(thread_count)
    openmp_hello();

    return 0;
}

void openmp_hello(void) {
    int my_thread_id = omp_get_thread_num(); // Obtener el ID del hilo actual
    int total_threads = omp_get_num_threads(); // Obtener el total de hilos en el equipo

    if (my_thread_id % 2 == 0) {
        printf("Saludos del hilo %d\n", my_thread_id);
    } else {
        printf("Feliz cumple numero %d!\n", my_thread_id);
    }
}
