#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    omp_set_num_threads(8);
    int num = 250;
    int num_sq = 0;

    #pragma omp parallel for
        for (int i = 1; i < num * 2; i++) {
            if (i % 2 == 1) {
                #pragma omp atomic
                    num_sq += i;
            }
        }

    printf("%d", num_sq);

}