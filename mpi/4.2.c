#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define SIZE 12

int main(int argc, char **argv) {

	int n_pr, amount;
	int rc;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &amount);
	{

		printf("pr<%d> size: %d\n", n_pr, amount);

		if (n_pr == 0) {

			int *x = (int *) malloc(sizeof(int) * SIZE);

			for (int i = 0; i < SIZE; i++) {
				*(x + i) = i;
			}

			for (int i = 0; i < SIZE; i++) {
				printf("x%d: %d\n", i, *(x + i));
			}

			amount--;
			for (int i = 0; i < amount; i++) {

				int to_send = SIZE / amount + 1;

				MPI_Send(x + (SIZE / amount + 1) * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
			}

			free(x);
		} else {

			MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &rc);

			int *x = (int *) calloc(rc, sizeof(int));

			MPI_Recv(x, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			for (int i = 0; i < rc; i++) {
				printf("pr<%d>: %d\n", n_pr, *(x + i));
			}
			printf("\n");
			free(x);
		}

	}
	MPI_Finalize();

	return 0;
	
}
