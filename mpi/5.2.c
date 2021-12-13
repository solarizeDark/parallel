#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define n 8
#define m 8

struct human {

    int          age;
    double    height;
    char    name[20];

};

int main(int argc, char **argv) {

    int n_pr, size;
    int rc;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Datatype human_t;

    int lengths[3] = { 1, 1, 20 };

    MPI_Aint displacements[3];
    struct human h;

    MPI_Aint base_address;
    MPI_Get_address(&h,             &base_address);
    MPI_Get_address(&h.age,     &displacements[0]);
    MPI_Get_address(&h.height,  &displacements[1]);
    MPI_Get_address(&h.name[0], &displacements[2]);

    displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    displacements[1] = MPI_Aint_diff(displacements[1], base_address);
    displacements[2] = MPI_Aint_diff(displacements[2], base_address);

    MPI_Datatype types[3] = { MPI_INT, MPI_DOUBLE, MPI_CHAR };

    MPI_Type_create_struct(3, lengths, displacements, types, &human_t);
    MPI_Type_commit(&human_t);

    {

        printf("pr<%d> size: %d\n", n_pr, size);

        if (n_pr == 0) {

            MPI_Datatype odd_even;
            MPI_Type_vector(n / 4, m, 4 * m, human_t, &odd_even);
            MPI_Type_commit(&odd_even);

            struct human * buffer = (struct human *) malloc(sizeof(struct human) * n * m);

            char name[20] = "temp_name";

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    (buffer + i * m + j)->age = i;
                    (buffer + i * m + j)->height = (double) i * 15 + 50;
                    strcpy((buffer + i * m + j)->name, name);
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    printf("%d ", (buffer + i * m + j)->age);
                }
                printf("\n");
            }
            printf("\n");

            MPI_Send(buffer,         1, odd_even, 1, 0, MPI_COMM_WORLD);
            MPI_Send(buffer +     m, 1, odd_even, 2, 0, MPI_COMM_WORLD);
            MPI_Send(buffer + 2 * m, 1, odd_even, 3, 0, MPI_COMM_WORLD);
            MPI_Send(buffer + 3 * m, 1, odd_even, 4, 0, MPI_COMM_WORLD);

            free(buffer);
        } else {

            struct human * buffer = (struct human *) malloc(sizeof(struct human) * n / 4 * m);

            MPI_Recv(buffer, n / 4 * m, human_t, 0, 0, MPI_COMM_WORLD, &status);

            for (int i = 0; i < n / 4; i++) {
                for (int j = 0; j < m; j++) {
                    printf("%d ", (buffer + i * m + j)->age);
                }
                printf("\n");
            }
            printf("\n");

        }

    }
    
    MPI_Finalize();
    return 0;

}