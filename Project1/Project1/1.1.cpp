#include <cstdlib>
#include <iostream>
#include "mpi.h" 
#define MAX 10 
using namespace std;

int main(int argc, char *argv[])
{
	int rank, size, n, ibeg, iend;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	n = (MAX - 1) / size + 1;
	ibeg = rank * n + 1;
	iend = (rank + 1)*n;
	for (int i = ibeg; i <= ((iend>MAX) ? MAX : iend); ++i) {
		for (int j = 1; j < MAX; ++j)
			cout << "The process #" << rank << ": " << i << "*" << j << "=" << i * j << endl;
	}
	MPI_Finalize();
}

