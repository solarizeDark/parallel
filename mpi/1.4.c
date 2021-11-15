#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	int n_pr, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	{

		printf("pr<%d> size: %d\n", n_pr, size);

		if (n_pr == 0) {
			int a[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
			// MPI_Bcast(a, 10, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Send(a, 10, MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(a, 10, MPI_INT, 2, 1, MPI_COMM_WORLD);
			MPI_Send(a, 10, MPI_INT, 3, 1, MPI_COMM_WORLD);
	
		} else {
			MPI_Status status;
			int rc;
			MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &rc);
			printf("pr<%d> recieved: %d\n", n_pr, rc);

			int *ptr = (int *) calloc(rc, sizeof(int));
			MPI_Recv(ptr, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			for (int *temp = ptr; temp != ptr + rc; temp++) {
				printf("%d ", *temp);
			}
			printf("\n");
			free(ptr);	
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
