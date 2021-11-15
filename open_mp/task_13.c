#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    srand(time(0));

    // 101101000111010100010111000101
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};

    omp_set_num_threads(8);
    int num = 0;
    // 2^29 - initial value
    int powers_of_two = 536870912;
    #pragma omp parallel for ordered
        for(int i = 0; i < 30; i++) {
                
                #pragma omp ordered 
                {
                    #pragma omp atomic
                    num += a[i] * powers_of_two;
                
                    printf("%d\t%d\t%d\n", omp_get_thread_num(), a[i], powers_of_two, num);
                    powers_of_two /= 2;
                }
        }

    printf("%d", num);

}