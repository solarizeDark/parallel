#include <stdio.h>
#include <omp.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    int a = 0;
    int b = 10;

    omp_set_num_threads(2);
    #pragma omp parallel private(a) firstprivate(b)
    {
        int id = omp_get_thread_num();
        a += id;
        b += id;
        printf("id: %d\ta: %d\tb: %d\n", id, a, b);
    }
    printf("\na: %d\tb: %d\n\n", a, b);

    omp_set_num_threads(4);
    #pragma omp parallel shared(a) private(b)
    {
        int id = omp_get_thread_num();
        a -= id;
        b -= id;
        printf("id: %d\ta: %d\tb: %d\n", id, a, b);
    }
    printf("\na: %d\tb: %d\n\n", a, b);

}