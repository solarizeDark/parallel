#include <stdio.h>
#include <omp.h>

omp_lock_t lock; 

int main(int argc, char *argv[]) {
    int i, n;

    omp_set_num_threads(8);

    #pragma omp parallel private (i, n)
    {
        n = omp_get_thread_num();
        #pragma omp
        for (i = omp_get_num_threads() - 1; i >= 0; i--) {
            #pragma omp barrier
            {
                if (omp_get_thread_num() == i) 
                    printf("ordered: Thread %d, i %d\n", n, i);
            }
        }
    }
    
}