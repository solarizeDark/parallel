#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {

	int n_pr, size;

	MPI_Status status;
	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	{
		printf("pr<%d> size: %d\n", n_pr, size);
		if (n_pr == 0) {
			int a[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
			MPI_Send(a, 10, MPI_INT, 1, 1, MPI_COMM_WORLD);
		} else {
			int b[10];
			MPI_Recv(b, 10, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			for (int i = 0; i < 10; i++) {
				printf("%d ", b[i]); 	
			}
			printf("\n");
		}

	}
	MPI_Finalize();

	return 0;
	
}
