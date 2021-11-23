#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define SIZE 10

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

			int *x = (int *) malloc(sizeof(int) * SIZE);
			int *y = (int *) malloc(sizeof(int) * SIZE);
			
			for (int i = 0; i < SIZE; i++) {
				*(x + i) = i;
				*(y + i) = -1 * i + 1;
			} 

			for (int i = 0; i < SIZE; i++) {
				printf("x%d: %d\ty%d: %d\n", i, *(x + i), i, *(y + i));
			}

			for (int i = 0; i < 3; i++) {

				int to_send = SIZE / 3;
				if ((i == 2) && (SIZE % 3 != 0)) {
					to_send += SIZE % 3;	
				} 
			
				MPI_Send(x + SIZE / 3 * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
				MPI_Send(y + SIZE / 3 * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);				
			}

			for (int i = 0; i < 3; i++) {

				int to_recieve = SIZE / 3;
				if ((i == 2) && (SIZE % 3 != 0)) {
					to_recieve += SIZE % 3;
				}
			
				MPI_Recv(x + SIZE / 3 * i, to_recieve, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);	
				MPI_Recv(y + SIZE / 3 * i, to_recieve, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);	
			}

			printf("\n");
			for (int i = 0; i < SIZE; i++) {
				printf("x%d: %d\ty%d: %d\n", i, *(x + i), i, *(y + i));
			}

			free(x);
			free(y);
		} else {

			MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &rc);

			int *x = (int *) calloc(rc, sizeof(int));
			int *y = (int *) calloc(rc, sizeof(int));
			
			MPI_Recv(x, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(y, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			for (int i = 0; i < rc; i++) {
				int temp = *(x + i); 
				*(x + i) = *(y + i);
				*(y + i) = temp;	
			}

			MPI_Send(x, rc, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(y, rc, MPI_INT, 0, 1, MPI_COMM_WORLD);
			free(x);
			free(y);
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
