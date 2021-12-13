#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 10

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Status status;

    int ranks[5] = {1, 3, 6, 8, 9};
    MPI_Group new_group;
    MPI_Group_incl(world_group, 5, ranks, &new_group);

    MPI_Comm new_communicator;
    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_communicator);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *buffer = (int *) malloc (sizeof(int) * n);

    if (rank == 1) {

        for (int i = 0; i < n; i++) *(buffer + i) = i;
        MPI_Bcast(buffer, n, MPI_INT, 0, new_communicator);
    }

    if (new_communicator != MPI_COMM_NULL) {

        MPI_Bcast(buffer, n, MPI_INT, 0, new_communicator);

        printf("rank: <%d>\n", rank);
        for (int i = 0; i < n; i++) printf("%d ", *(buffer + i));
        printf("\n");

        if (rank == 9) {
            MPI_Send(buffer, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }

    }

    if (rank == 0) {

        MPI_Recv(buffer, n, MPI_INT, 9, 1, MPI_COMM_WORLD, &status);

        printf("rank: <%d>\n", rank);

        for (int i = 0; i < n; i++) printf("%d ", *(buffer + i));
        printf("\n");

    }

    MPI_Finalize();
    return 0;
}