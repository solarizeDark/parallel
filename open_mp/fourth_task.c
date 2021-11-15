#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

// author: Fedusiv

int min(int arr[], int size) {
    int min = arr[0];
    for(int i = 0; i < size; i++)
        if(arr[i] < min)
            min = arr[i];
    return min;
}

int max(int arr[], int size) {
    int max = arr[0];
    for(int i = 0; i < size; i++)
        if(arr[i] > max)
            max = arr[i];
    return max;
}

int main(int argc, char *argv[]) {

    int a[10], b[10];

    for(int i = 0; i < 10; i++) a[i] = b[i] = i; 

    omp_set_num_threads(2);
    int min_a = -1;
    int max_b = -1;

    #pragma omp parallel
    {
        if (omp_get_thread_num() == 0) min_a = min(a, 10);
        else max_b = max(b, 10);
    }

    printf("%d\t%d", min_a, max_b);

}