#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 12
#define M 4
#define LENGTH 48

void print(int *v, int length) {
	int cnt = 0;
	for(int *p = v; p != v + length; p++) {
		printf("%d ", *p);
		if (++cnt % M == 0) printf("\n");
	}
	printf("\n");
}

int *random_vector(int length) {

	int *v = (int *) malloc(sizeof(int) * length);
	int flag = 0;
	for(int *p = v; p != v + length; p++) {
		*p = (random() % 10 + 1) * (flag++ % 2 == 0 ? 1 : -1);
	}

	return v; 
	
}

void res_counter(int *matrix_chunk, int *vector, int *res, int length) {

	int cnt = 0;
	for (int *m = matrix_chunk, *v = vector, *r = res; m != matrix_chunk + length; m++) {
		*r += *m * *(v + (cnt++ % M));
		if (cnt % M == 0 && cnt != 0) r++;
	}
	
}

int main(int argc, char **argv) {
	srand(time(NULL));

	int n_pr, amount;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &n_pr);
	MPI_Comm_size(MPI_COMM_WORLD, &amount);
	{
		printf("pr<%d> size: %d\n", n_pr, amount);

		int *matrix_chunk = (int *) malloc(sizeof(int) * (LENGTH / amount));
		int *res = (int *) calloc(N / amount, sizeof(int));

		if (n_pr == 0) {

			int *ans = (int *) malloc(sizeof(int) * N);
			int *matrix = random_vector(LENGTH);
			int *vector = random_vector(M);

			print(matrix, LENGTH);
			print(vector, M);

			MPI_Scatter(matrix, LENGTH / amount, MPI_INT, matrix_chunk, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(vector, M, MPI_INT, 0, MPI_COMM_WORLD);

			res_counter(matrix_chunk, vector, res, LENGTH / amount);
			
			MPI_Gather(res, N / amount, MPI_INT, ans, N / amount, MPI_INT, 0, MPI_COMM_WORLD);
			print(ans, N);				
			
			free(ans);
			free(vector);	

		} else {

			int *vector = (int *) malloc(sizeof(int) * M);

	        MPI_Scatter(NULL, 1, MPI_INT, matrix_chunk, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);    
			MPI_Bcast(vector, M, MPI_INT, 0, MPI_COMM_WORLD);

			res_counter(matrix_chunk, vector, res, LENGTH / amount);
			
			MPI_Gather(res, N / amount, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);	

			free(vector);		
		}			

		free(matrix_chunk);
		free(res);

	}

	MPI_Finalize();

	return 0;
	
}
