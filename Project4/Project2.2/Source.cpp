#include <cstdlib>
#include <iostream>
#include "mpi.h" 
#include <time.h>
#define MAX_RAND 21
#define ARRAY_SIZE 10


using namespace std;

int getCountByRank(int rank, int size) {
	int forOne = ARRAY_SIZE / size;
	int addition = ARRAY_SIZE % size;
	if (rank == size - 1 && addition != 0) {
		return forOne + addition;
	}
	return forOne;
}

int getAddition(int size){
	return ARRAY_SIZE % size;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int length = getCountByRank(rank,size);
	int max = getCountByRank(size - 1, size);
	int *ai = (int*)malloc(ARRAY_SIZE * sizeof(int)+1);
	int *rbuf = (int*)malloc(length * sizeof(int)+1);


	if (rank == 0) {
		for (int i = 0; i<ARRAY_SIZE; ++i) {
			ai[i] = rand() % MAX_RAND - ARRAY_SIZE/2;
			printf("init a[%d]=%d\n", i, ai[i]);
		}

		
		//MPI_Bcast(a, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
		//for (int i = 1; i<size; ++i) {
			//int length1 = getCountByRank(i, size);
			//MPI_Send(a, length1, MPI_INT, i, 0, MPI_COMM_WORLD);

			//if (i<size - 1) {
			//	a += length1;
			//}
		//}
	}

	MPI_Scatter(ai, length, MPI_INT, rbuf, length, MPI_INT, 0, MPI_COMM_WORLD);

	int sum = 0;
	for (int i = 0; i<length; ++i) {
		if (rbuf[i] > 0 && rbuf[i] % 2 == 0){
				sum++;
		}
	}
	for (int i = ARRAY_SIZE - 1; i > ARRAY_SIZE -1 - getAddition(size); i--){
		if (ai[i] > 0 && ai[i] % 2 == 0){
			sum++;
		}
	}
		int globalSum = 0;
		MPI_Reduce(&sum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0){
			cout << "Number of even positive elements: " << globalSum << endl;

		}
	

	MPI_Finalize();
}

