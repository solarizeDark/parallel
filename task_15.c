#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include<stdbool.h>  

// author: Fedusiv

int main(int argc, char *argv[]) {

    omp_set_num_threads(8);
    int num = 250;

    #pragma omp parallel for
        for (int i = 1; i < num; i++) {
            bool flag = true;
            for (int j = 2; j < sqrt(i); j++) {
                if (i % j == 0) {
                    flag = false; 
                    break;
                }
            }
            if (flag) printf("%d\n", i);
        }

}