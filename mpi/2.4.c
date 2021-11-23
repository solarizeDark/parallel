#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define X 10
#define Y 4
#define Z 10

void print_matrix(int *ma, int n, int m) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {	
			printf(*(ma + i * m + j) < 100 ? (*(ma + i * m + j) < 10 ? "  %d " : " %d ") : "%d ", *(ma + i * m + j) );
			if (j == m - 1) printf("\n");
		}
	}
	printf("\n");

}

int* random_matrix(int n, int m) {

	int *ma = (int *) malloc(sizeof(int) * n * m);

	for (int i = 0; i < n; i++) 
		for (int j = 0; j < m; j++)
			*(ma + i * m + j) = random() % n + 1;

	print_matrix(ma, n, m);	
	return ma;
	
}

int * mul(int *x, int *y, int lX) {

	int lY, lZ; 
	lY = Y;
	lZ = Z;

	int *z = (int *) calloc(lX * lZ, sizeof(int));

	for (int i = 0; i < lX; i++) {
		
		for (int j = 0; j < lZ; j++) {

			int lsum = 0;
			
			for (int k = 0; k < lY; k++) {
				lsum += *(x + i * lY + k) * *(y + k * lZ + j);	
			}
			*(z + lZ * i + j) = lsum;
		}
	}
	return z;			
}

int main(int argc, char **argv) {
	srand(time(NULL));

	int n_pr, _amount;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &_amount);
	{
		int rc;
		printf("pr<%d> size: %d\n", n_pr, _amount);

		if (n_pr == 0) {

			int amount = _amount;
			amount--;
			
			int *a = random_matrix(X, Y);
			int *b = random_matrix(Y, Z);
			
			for (int i = 0; i < amount; i++) {

				int to_send = X / amount * Y;
				if ((i == (amount - 1)) && (X % amount != 0)) {
					to_send += X % amount * Y;	
				} 

				MPI_Send(a + X / amount * Y * i, to_send, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
				MPI_Send(b, Y * Z, MPI_INT, i + 1, 1, MPI_COMM_WORLD);

			}
		
			int *z = (int *) malloc(sizeof(int) * X * Z);
			
			for (int i = 0; i < amount; i++) {

				int to_recv = X / amount * Z;
				if ((i == (amount - 1)) && (X % amount != 0)) {
					to_recv += X % amount * Z;	
				}
				MPI_Recv(z + X / amount * Z * i, to_recv, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);
			}
			print_matrix(z, X, Z);

			free(a);
			free(b);
			free(z);
			
		} else {

			int lX = X / (_amount - 1) + (n_pr == _amount - 1 ? 1 : 0); 

			int *x = (int *) calloc(lX * Y, sizeof(int));
			int *y = (int *) calloc(Y * Z, sizeof(int));
			
			MPI_Recv(x, lX * Y, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(y, Y * Z, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
												
			MPI_Send(mul(x, y, lX), lX * Z, MPI_INT, 0, 1, MPI_COMM_WORLD);
			free(x);
			free(y);
		}
		
	}
	MPI_Finalize();

	return 0;
	
}
