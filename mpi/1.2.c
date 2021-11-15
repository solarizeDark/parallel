#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {

	int n_pr, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	{
		printf("pr<%d> size: %d\n", n_pr, size);
	}
	MPI_Finalize();

	return 0;
	
}
