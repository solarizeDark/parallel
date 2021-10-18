#include <stdio.h>
#include <omp.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    int a[10];
    int b[10];

    for(int i = 0; i < 10; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    double mean_a = 0;
    double mean_b = 0;

    double m_a;

    omp_set_num_threads(10);
    #pragma omp parallel reduction (+ : m_a)
    {
        m_a = (double) a[omp_get_thread_num()] / 10;
    }

    printf("%f\n\n", m_a);

    #pragma omp parallel for reduction (+ : mean_a) reduction (+ : mean_b)
        for(int i = 0; i < 10; i++) 
        {
            mean_a = (double) a[i] / 10;
            mean_b = (double) b[i] / 10;
        }

    printf("%f %f", mean_a, mean_b);

}