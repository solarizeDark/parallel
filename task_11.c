#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    srand(time(0));

    int a[30];
    for (int i = 0; i < 30; i++)
            a[i] = rand() % 22;

    for (int i = 0; i < 30; i++)
            printf("%d, ", a[i]);

    printf("\n");
    int max_div_by7 = -1;

    omp_set_num_threads(8);

    #pragma omp parallel for
        for(int i = 0; i < 30; i++) 
            if (a[i] % 7 == 0 && a[i] > max_div_by7) {
                #pragma omp critical
                    max_div_by7 = a[i];
            }

    printf("%d", max_div_by7);

}