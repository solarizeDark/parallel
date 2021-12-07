#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

//#define SIZE 100
#define TIMES 10

int main(int argc, char **argv) {

	int n_pr, size;
	int rc;

	int SIZE = atoi(argv[1]);
	printf("%d\n", SIZE);
	
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	{

		printf("pr<%d> size: %d\n", n_pr, size);

		if (n_pr == 0) {

			double t1, t2;

			int *x = (int *) malloc(sizeof(int) * SIZE);
			int *y = (int *) malloc(sizeof(int) * SIZE);

						
			for (int i = 0; i < SIZE; i++) {
				*(x + i) = i;
			} 

			t1 = MPI_Wtime();
			for (int i = 0; i < TIMES; i++) {
				MPI_Send(x, SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD);
				MPI_Recv(y, SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			}
			t2 = MPI_Wtime();

			printf("pr<%d> %f\n", n_pr, t2 - t1);

			free(x);
			free(y);
		} else {
			int *x = (int *) malloc(sizeof(int) * SIZE);

			for (int i = 0; i < TIMES; i++) {
				MPI_Recv(x, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);		
				MPI_Send(x, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
			}
			
			free(x);
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
