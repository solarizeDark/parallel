#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// author: Fedusiv

int main(int argc, char *argv[]) {

    srand(time(0));

    int d[6][8];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 8; j++)
            d[i][j] = rand() % 20;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d, ", d[i][j]);
        }
        printf("\n");
    }

    int min = d[0][0];
    int max = d[0][0];

    omp_set_num_threads(8);
    #pragma omp critical
    {
        #pragma omp parallel for collapse(2)
            for(int i = 0; i < 6; i++) {
                for(int j = 0; j < 8; j++) {
                    if (d[i][j] < min) min = d[i][j];
                    if (d[i][j] > max) max = d[i][j];
                     
                }
            }
            printf("%d\n", omp_get_thread_num());
    }

    printf("%d %d", min, max);

}