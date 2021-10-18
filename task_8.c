#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
// author: Fedusiv
void print_vector(int *, int);

int** matrix_creator(int n, int m) {
    int ** rows = calloc(n, sizeof(int*));
    for(int i = 0; i < n; i++) {
        rows[i] = calloc(m, sizeof(int));
        for (int j = 0; j < m; j++) {
            rows[i][j] = i + j;
        }
    }  
    return rows;
}

int* vector_creator(int n) {
    int * p = calloc(n, sizeof(int));
    int * temp = p;
    for(int pos = 0; p != temp + n; p++, pos++)
        *p = pos;
    return temp;
}

int* matrix_by_vector_multiplyer(int ** matrix, int * vector, int n, int m) {
    int * res = calloc(n, sizeof(int));
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            res[i] += matrix[i][j] * vector[j];
        }
    }
    return res;
}

int* matrix_by_vector_multiplyer_parallel(int ** matrix, int * vector, int n, int m) {
    int * res = calloc(n, sizeof(int));
    
    #pragma omp parallel 
    {
        #pragma omp for collapse(2)
        
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < m; j++) {
                    res[i] += matrix[i][j] * vector[j];
                }
            }
    }
    return res;
}

void print_matrix(int ** matrix, int n, int m) {
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_vector(int * vector, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n\n");
}

void calloc_time(int n) {
    double start = omp_get_wtime();
    int * res = calloc(n, sizeof(int));
    double end = omp_get_wtime();
    printf("Calloc time execution: %f\n\n", end - start);
}

int main(int argc, char *argv[]) {

    int n, m;
    scanf("%d %d", &n, &m);

    calloc_time(n);
    printf("Tick: %f\n\n", omp_get_wtick());


    int ** matrix = matrix_creator(n, m);
    int * vector = vector_creator(m);

    double start_parallel, end_parallel;
    start_parallel = omp_get_wtime();
    int * res = matrix_by_vector_multiplyer_parallel(matrix, vector, n, m);
    end_parallel = omp_get_wtime();
    printf("Parallel: %f\n", end_parallel - start_parallel);

    double start_sequent, end_sequent;
    start_sequent = omp_get_wtime();
    int * res_seq = matrix_by_vector_multiplyer(matrix, vector, n, m);
    end_sequent = omp_get_wtime();
    printf("Sequent: %f", end_sequent - start_sequent);
}