#include <cstdlib>
#include <iostream>
#include "mpi.h" 
#include <time.h>
#define MAX 10

using namespace std;

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int ai = rank * 2 + 1;
	int bi = ai + 1;

	printf("a%d=%d b%d=%d\n", rank, ai, rank, bi);
	int an, bn;
	switch (rank) {
	case 0:
		MPI_Send(&ai, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		MPI_Send(&bi, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

		MPI_Recv(&an, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&bn, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		break;
	case 1:
		MPI_Send(&ai, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&bi, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);

		MPI_Recv(&an, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&bn, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		break;
	case 2:
		MPI_Send(&ai, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
		MPI_Send(&bi, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		MPI_Recv(&an, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&bn, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		break;
	case 3:
		MPI_Send(&ai, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&bi, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

		MPI_Recv(&an, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&bn, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		break;
	}

	printf("c%d=%d+%d=%d\n", rank, an, bn, an + bn);
	MPI_Finalize();
}

