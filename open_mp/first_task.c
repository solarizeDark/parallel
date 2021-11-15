#include <stdio.h>
#include <omp.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    omp_set_num_threads(8);
    #pragma omp parallel
    {
        printf("Hello, World!\tid: %d\tamount: %d\n", omp_get_thread_num(), omp_get_num_threads());
    }

}