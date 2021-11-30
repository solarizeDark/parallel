#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 120

void print(int *v) {
	for(int *p = v; p != v + LENGTH; p++) printf("%d ", *p);
	printf("\n");
}

int *random_vector() {

	int *v = (int *) malloc(sizeof(int) * LENGTH);
	int flag = 0;
	for(int *p = v; p != v + LENGTH; p++) {
		*p = (random() % 10 + 1) * (flag++ % 2 == 0 ? 1 : -1);
	}

	print(v);
		
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

		int sum = 0, res = 0;
		int *rc = (int *) malloc(sizeof(int) * LENGTH / amount);

		if (n_pr == 0) {

			int *v = random_vector();
			MPI_Scatter(v, LENGTH / amount, MPI_INT, rc, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
			
		} else {
	        MPI_Scatter(NULL, 1, MPI_INT, rc, LENGTH / amount, MPI_INT, 0, MPI_COMM_WORLD);
	    }

		for (int *p = rc; p != rc + LENGTH / amount; p++)
			res += (*p < 0 ? -1 : 1) * *p;
		printf("res: %d\n", res);
		
		MPI_Reduce(&res, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (n_pr == 0) printf("SUM: %d\n", sum);
	}
	MPI_Finalize();

	return 0;
	
}
