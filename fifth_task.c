#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

// author: Fedusiv

void print(int n, int m, int arr[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

int min(int n, int m, int arr[n][m]) {
    int min = arr[0][0];
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            if(arr[i][j] < min) min = arr[i][j]; 
    return min;
}

int max(int n, int m, int arr[n][m]) {
    int max = arr[0][0];
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            if(arr[i][j] > max) max = arr[i][j]; 
    return max;
}

double mean(int n, int m, int arr[n][m]) {
    double sum = 0;
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
                sum += arr[i][j];
    return sum / n / m; 
}

int div3_count(int n, int m, int arr[n][m]) {
    int cnt = 0;
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
                if(arr[i][j] % 3 == 0) cnt++;
    return cnt; 
}

int main(int argc, char *argv[]) {

    srand(time(NULL));

    int d[6][8];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 8; j++)
            d[i][j] = rand();

    print(6, 8, d);

    int min_d = -1, max_d = -1, cnt = -1, mean_d = -1; 
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                
                mean_d = mean(6, 8, d);
            }
            #pragma omp section
            {
                min_d = min(6, 8, d);
                max_d = max(6, 8, d);
            }
            #pragma omp section
            {
                cnt = div3_count(6, 8, d);
            }
        }
    }

    printf("mean: %d\nmin, max: %d, %d\ncount: %d", mean_d, min_d, max_d, cnt);

}