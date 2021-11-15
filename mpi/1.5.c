#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define SIZE 10

int main(int argc, char **argv) {

	int n_pr, size;
	int rc;

	int a = 1, b = 1;
	
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	{

		printf("pr<%d> size: %d\n", n_pr, size);

		if (n_pr == 0) {

			int *x = (int *) malloc(sizeof(int) * SIZE);
			int *y = (int *) malloc(sizeof(int) * SIZE);
			
			for (int i = 0; i < SIZE; i++) {
				*(x + i) = i;
				*(y + i) = i;
			} 

			for (int i = 0; i < 3; i++) {

				int to_send = SIZE / 3;
				if ((i == 2) && (SIZE % 3 != 0)) {
					to_send += SIZE % 3;	
				} 
			
				MPI_Send(x + SIZE / 3 * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
				MPI_Send(y + SIZE / 3 * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);				
			}
			
			int *z = (int *) malloc(sizeof(int) * SIZE);
			for (int i = 0; i < 3; i++) {

				int to_recieve = SIZE / 3;
				if ((i == 2) && (SIZE % 3 != 0)) {
					to_recieve += SIZE % 3;	
				}
			
				MPI_Recv(z + SIZE / 3 * i, to_recieve, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);	
			}

			printf("\n");
			for (int *t = z; t != z + SIZE; t++) {
				printf("%d\n", *t);
			}

			free(x);
			free(y);
			free(z);
		} else {

			MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &rc);

			int *x = (int *) calloc(rc, sizeof(int));
			int *y = (int *) calloc(rc, sizeof(int));
			int *z = (int *) calloc(rc, sizeof(int));
			
			MPI_Recv(x, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(y, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			for (int i = 0; i < rc; i++) {
				*(z + i) = *(x + i) * a + *(y + i) * b;
			}

			MPI_Send(z, rc, MPI_INT, 0, 1, MPI_COMM_WORLD);
			free(x);
			free(y);
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
