#include <stdio.h>
#include <omp.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    int a[12], b[12], c[12];

    omp_set_num_threads(3);
    #pragma omp parallel 
    {
        #pragma omp single
            printf("Max threads: %d\n", omp_get_max_threads());

        #pragma omp for schedule (static, 2)
            for(int i = 0; i < 12; i++) {
                printf("Thread #%d\titeration: %d\n", omp_get_thread_num(), i);
                a[i] = i;
                b[i] = i * 2;
            }
    }

    printf("\n");

    omp_set_num_threads(4);
    #pragma omp parallel 
    {
        #pragma omp single
            printf("Max threads: %d\n", omp_get_max_threads());

        #pragma omp for schedule (dynamic, 2)
            for(int i = 0; i < 12; i++) {
                printf("Thread #%d\titeration: %d\n", omp_get_thread_num(), i);
                c[i] = a[i] + b[i];
            }
    }

}