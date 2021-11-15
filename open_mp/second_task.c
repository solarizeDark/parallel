#include <stdio.h>
#include <omp.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    omp_set_num_threads(3);
    #pragma omp parallel if (omp_get_max_threads() > 1) 
    {
        if (omp_in_parallel())
        {
            #pragma omp single
            {
                printf("Max threads: %d\n", omp_get_max_threads());
            }

            printf("First\tid: %d\tamount: %d\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }

    printf("\n");

    omp_set_num_threads(1);
    #pragma omp parallel if (omp_get_max_threads() > 1)
    {
        if (omp_in_parallel())
        {
            printf("Max threads: %d\n", omp_get_max_threads());
            printf("Second\tid: %d\tamount: %d\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }

}