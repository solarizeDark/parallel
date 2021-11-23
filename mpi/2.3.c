#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 10

void print_matrix(int *m) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {	
			printf(*(m + SIZE * i + j) < 10 ? " %d " : "%d ", *(m + SIZE * i + j) );
			if (j % (SIZE - 1) == 0 && j != 0) printf("\n");
		}
	}
	printf("\n");

}

int *random_matrix() {

	int *m = (int *) malloc(sizeof(int) * SIZE * SIZE);

	for (int i = 0; i < SIZE; i++) 
		for (int j = 0; j < SIZE; j++)
			*(m + SIZE * i + j) = random() % SIZE;

	print_matrix(m);	
	return m;
	
}

int main(int argc, char **argv) {
	srand(time(NULL));

	int n_pr, amount;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &amount);
	{
		int rc;
		printf("pr<%d> size: %d\n", n_pr, amount);

		if (n_pr == 0) {

			amount--;
			
			int *a = random_matrix();
			int *b = random_matrix();
			
			for (int i = 0; i < amount; i++) {

				int to_send = SIZE * SIZE / amount;
				if ((i == (amount - 1)) && (SIZE % amount != 0)) {
					to_send += (SIZE * SIZE) % amount;	
				} 

				MPI_Send(a + SIZE * SIZE / amount * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
				MPI_Send(b + SIZE * SIZE / amount * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);

			}
		
			int *z = (int *) malloc(sizeof(int) * SIZE * SIZE);
			
			for (int i = 0; i < amount; i++) {

				int to_recv = SIZE * SIZE / amount;
				if ((i == (amount - 1)) && (SIZE % amount != 0)) {
					to_recv += (SIZE * SIZE) % amount;	
				}
				
				MPI_Recv(z + SIZE * SIZE / amount * i, to_recv, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);
			}
			print_matrix(z);

			free(a);
			free(b);
			free(z);
		} else {

			MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &rc);

			int *x = (int *) calloc(rc, sizeof(int));
			int *y = (int *) calloc(rc, sizeof(int));
			
			MPI_Recv(x, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(y, rc, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			int *z = (int *) calloc(rc, sizeof(int));

			for (int i = 0; i < rc; i++)
				*(z + i) = *(x + i) * *(y + i);
					
			MPI_Send(z, rc, MPI_INT, 0, 1, MPI_COMM_WORLD);
			free(x);
			free(y);
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
