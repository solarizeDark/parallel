#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 8
#define m 8

int main(int argc, char **argv) {

    int n_pr, size;
    int rc;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    {

        printf("pr<%d> size: %d\n", n_pr, size);

        if (n_pr == 0) {

            MPI_Datatype odd_even;
            MPI_Type_vector(n/2, m, 2 * m, MPI_INT, &odd_even);
            MPI_Type_commit(&odd_even);

            int *buffer = (int *) malloc(sizeof(int) * n * m);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                        *(buffer + i * m + j) = i;
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    printf("%d ", *(buffer + i * m + j));
                }
                printf("\n");
            }
            printf("\n");

            MPI_Send(buffer, 1, odd_even, 1, 0, MPI_COMM_WORLD);
            MPI_Send(buffer + m, 1, odd_even, 2, 0, MPI_COMM_WORLD);

            free(buffer);
        } else {

            int *buffer = (int *) malloc(sizeof(int) * n / 2 * m);

            MPI_Recv(buffer, n / 2 * m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < n / 2; i++) {
                for (int j = 0; j < m; j++) {
                    printf("%d ", *(buffer + i * m + j));
                }
                printf("\n");
            }

        }

    }
    MPI_Finalize();

    return 0;

}
