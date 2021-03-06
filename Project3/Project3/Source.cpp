#include <cstdlib>
#include <iostream>
#include "mpi.h" 
#include <time.h>
#include <windows.h>
#define MAX_RAND 2
#define ROWS 7
#define COLUMNS 6

using namespace std;

int **matrixInit() {
	int *data = (int *)malloc(ROWS*COLUMNS * sizeof(int));
	int **array = (int **)malloc(ROWS * sizeof(int*));
	for (int i = 0; i<ROWS; i++)
		array[i] = &(data[COLUMNS*i]);

	return array;
}

int getCountByRank(int rank, int size) {
	int count = ROWS / size;
	int addition = ROWS % size;
	if (rank < addition && addition != 0) {
		return count + 1;
	}
	return count;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int** matrix = matrixInit();

	if (rank == 0) {
		cout << "Matrix: " << endl;
		for (int i = 0; i<ROWS; ++i) {
			for (int j = 0; j<COLUMNS; ++j) {
				matrix[i][j] = rand() % MAX_RAND;
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
		for (int i = 1; i<size; ++i) {
			MPI_Send(&(matrix[0][0]), ROWS*COLUMNS, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&(matrix[0][0]), ROWS*COLUMNS, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	int dif = 0;
	int count = getCountByRank(rank, size);
	for (int i = 0; i < count; i++) {
		int row = rank + i * size;
		for (int col = 0; col < COLUMNS / 2; col++) {
			matrix[row][col * 2] = (matrix[row][col * 2] + matrix[row][col * 2 + 1]) % 2;
		}
		for (int col = 0; col < COLUMNS-1; col++) {
			if (matrix[row][col] != matrix[row][col + 1])
				dif++;
		}
	}
	
	if (rank != 0) {
		MPI_Send(&(matrix[0][0]), ROWS*COLUMNS, MPI_INT, 0, 1, MPI_COMM_WORLD);
		MPI_Send(&dif, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	else {
		int gettedDif;
		for (int i = 1; i<size; ++i) {
			int** matrix2 = matrixInit();
			MPI_Recv(&(matrix2[0][0]), ROWS*COLUMNS, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&gettedDif, ROWS*COLUMNS, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			dif += gettedDif;
			int count = getCountByRank(i, size);

			for (int j = 0; j < count; j++) {
				int row = i + j * size;
				for (int col = 0; col < COLUMNS / 2; col++) {
					matrix[row][col * 2] = matrix2[row][col * 2];
				}
			}
			free(matrix2[0]);
			free(matrix2);
		}
	}

	if (rank == 0) {
		cout << "Matrix2: " << endl;
		for (int i = 0; i<ROWS; ++i) {
			for (int j = 0; j<COLUMNS; ++j) {
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << "Different num:" << dif;
	}

	free(matrix[0]);
	free(matrix);

	MPI_Finalize();
}