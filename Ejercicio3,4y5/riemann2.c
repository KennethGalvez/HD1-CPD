#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);
double trapezoides(double a, double b, int n, double *suma_global);

int main(int argc, char* argv[]) {
    double integral;
    double a = 1.0; // por defecto
    double b = 40.0; // por defecto
    int n = 1; //por defecto es 1
    double suma_global = 0.0;

    if(argc > 3) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
        n = strtol(argv[3], NULL, 10);
    }

    integral = trapezoides(a, b, n, &suma_global);

    printf("Con %d trapecios, nuestra aproximacion de la integral de %f a %f es = %.10f\n", n, a, b, integral);
    printf("Suma global acumulada: %.10f\n", suma_global);

    return 0;
}

double trapezoides(double a, double b, int n, double *suma_global) {
    double h = (b-a)/n;
    double integral = 0.0;

#pragma omp parallel shared(a, b, h, n, suma_global) reduction(+:integral)
    {
        double suma_local = 0.0;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int n_local = n / num_threads;
        double a_local = a + id * n_local * h;
        double b_local = a_local + n_local * h;
        printf("Hilo %d de %d hilos. n_local: %d, a_local: %f, b_local: %f\n", id, num_threads, n_local, a_local, b_local);
        suma_local += (f(a_local) + f(b_local)) / 2.0;
#pragma omp for
        for(int k = 1; k <= n_local-1; k++) {
            suma_local += f(a_local + k*h);
        }
#pragma omp critical
        {
            *suma_global += suma_local;
        }
        integral += suma_local;
    }

    integral *= h;
    return integral;
}

double f(double x) {
    return x*x;
}
