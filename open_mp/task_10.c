#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    srand(time(0));

    int a[30];
    for (int i = 0; i < 30; i++)
            a[i] = rand() % 20;

    for (int i = 0; i < 30; i++)
            printf("%d, ", a[i]);

    printf("\n");
    int count = 0;

    omp_set_num_threads(8);

    #pragma omp parallel for
        for(int i = 0; i < 30; i++) 
            if (a[i] % 9 == 0) {
                #pragma omp atomic 
                    count++;
            }

    printf("%d", count);

}