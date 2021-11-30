#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 12
#define M 10
#define LENGTH 120

void print(int *v, int length) {

	int cnt = 0;
	for(int *p = v; p != v + length; p++) {
		printf("%d ", *p);
		if (++cnt % M == 0) printf("\n");	
	}
	printf("\n");
}

int *random_vector() {

	int *v = (int *) malloc(sizeof(int) * LENGTH);
	int flag = 0;
	for(int *p = v; p != v + LENGTH; p++) {
		*p = (random() % 10 + 1) * (flag++ % 2 == 0 ? 1 : -1);
	}

	print(v, LENGTH);
		
	return v; 
	
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

		int max = 0;
		int *rc1 = (int *) malloc(sizeof(int) * LENGTH / amount);
		int *rc2 = (int *) malloc(sizeof(int) * LENGTH / amount);
		int *res = (int *) calloc(LENGTH / amount, sizeof(int));

		if (n_pr == 0) {

			int *matrix1 = random_vector();
			int *matrix2 = random_vector();

			MPI_Scatter(matrix1, LENGTH / amount, MPI_INT, rc1, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Scatter(matrix2, LENGTH / amount, MPI_INT, rc2, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);

		} else {
	        MPI_Scatter(NULL, 1, MPI_INT, rc1, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
	        MPI_Scatter(NULL, 1, MPI_INT, rc2, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
	    }

		int cnt = 0;
		for (int *m1 = rc1, *m2 = rc2, *r = res; m1 != rc1 + LENGTH / amount; m1++, m2++, r++) {
			*r = *m1 * *m2;
		}

		if (n_pr == 0) {

			int *aob = (int *) malloc(sizeof(int) * LENGTH);
			MPI_Gather(res, LENGTH / amount, MPI_INT, aob, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
			print(aob, LENGTH);
			free(aob);
			
		} else {
			MPI_Gather(res, LENGTH / amount, MPI_INT, NULL, 1, MPI_INT, 0, MPI_COMM_WORLD);
		}

		free(rc1);
		free(rc2);
		free(res);
		
	}
	MPI_Finalize();

	return 0;
	
}
