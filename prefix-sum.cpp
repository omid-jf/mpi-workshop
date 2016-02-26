/*
╔════════════════════════════════════════╗
║               Prefix Sum               ║
║         Written by Omid Jafari         ║
║          http://omidjafari.ir          ║
╚════════════════════════════════════════╝
*/
#include <iostream>
#include <mpi.h>

using namespace std;

#define LIST_SIZE 8
#define PROCS 4

void main(int argc, char **argv)
{
	int rank;
	int nums[LIST_SIZE];
	int chunk[LIST_SIZE/PROCS] = { 0 };
	int step = 1;
	int localsum = 0;
	int recv = 0;
	int result = 0;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Produce the initial array
	if (rank == 0) {
		cout << "Initial array: ";
		for (int i = 0; i < LIST_SIZE; i++) {
			nums[i] = i + 1;
			cout << nums[i] << " ";
		}
		cout << endl;
	}

	// Scatter the initial array to all procs
	MPI_Scatter(nums, LIST_SIZE / PROCS, MPI_INT, chunk, LIST_SIZE / PROCS, MPI_INT, 0, MPI_COMM_WORLD);
	
	// Calculate the sum of each local chunk
	for (int i = 0; i < LIST_SIZE / PROCS; i++)
		result = localsum += chunk[i];

	// Communitcations
	while (step < PROCS)
	{
		if (rank + step < PROCS)
			MPI_Send(&localsum, 1, MPI_INT, rank + step, 1, MPI_COMM_WORLD);

		if (rank - step >= 0) {
			MPI_Recv(&recv, 1, MPI_INT, rank - step, 1, MPI_COMM_WORLD, &status);
			result += recv;
		}

		step++;
	}

	cout << "I'm " << rank << " and my sum is " << result << endl;

	MPI_Finalize();
}
