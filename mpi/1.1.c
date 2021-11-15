#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {

	int n_pr;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	{
		printf("pr<%d> Hello World\n", n_pr);
	}
	MPI_Finalize();

	return 0;
	
}
