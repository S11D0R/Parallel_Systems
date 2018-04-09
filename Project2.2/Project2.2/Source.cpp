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

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int max = getCountByRank(size - 1, size);
	int *ai = (int*)malloc(ARRAY_SIZE * sizeof(int) + 1);

	int length = getCountByRank(rank, size);
	if (rank == 0) {
		for (int i = 0; i<ARRAY_SIZE; ++i) {
			ai[i] = rand() % MAX_RAND - ARRAY_SIZE;
			printf("init a[%d]=%d\n", i, ai[i]);
		}

		int* a = ai;
		a += length;
		for (int i = 1; i<size; ++i) {
			int length1 = getCountByRank(i, size);
			MPI_Send(a, length1, MPI_INT, i, 0, MPI_COMM_WORLD);

			if (i<size - 1) {
				a += length1;
			}
		}
	}
	
	int sum = 0;
	if (rank != 0) {
		MPI_Recv(ai, max, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (int i = 0; i<length; ++i) {
			if (ai[i] > 0 && ai[i] % 2 == 0)
				sum++;
		}
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 0; i<length; ++i) {
			if (ai[i] > 0 && ai[i] % 2 == 0)
				sum++;
		}

		int gettedNum = 0;
		for (int i = 1; i<size; ++i) {
			MPI_Recv(&gettedNum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += gettedNum;
		}
		cout << "Number of even positive elements: " << sum << endl;
	}

	MPI_Finalize();
}