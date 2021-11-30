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
		int *rc = (int *) malloc(sizeof(int) * LENGTH / amount);
		int *res = (int *) calloc(N / amount, sizeof(int));

		if (n_pr == 0) {

			int *v = random_vector();
			MPI_Scatter(v, LENGTH / amount, MPI_INT, rc, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
			
		} else {
	        MPI_Scatter(NULL, 1, MPI_INT, rc, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
	    }

		int cnt = 0;
		for (int *p = rc, *r = res; p != rc + LENGTH / amount; p++) {
			*r += (*p < 0 ? -1 : 1) * *p;
			if (++cnt % M == 0) r++;
		}
		print(res, 3);
		
		MPI_Reduce(res, &max, N / amount, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
		if (n_pr == 0) printf("MAX: %d\n", max);
	}
	MPI_Finalize();

	return 0;
	
}
