// 
// Torbert, 10 October 2012
// 
// mpicc mpidemo.c
// mpirun -np 4 a.out
// 
// mpirun -np 4 -machinefile hosts.txt a.out
// 
#include <stdio.h>
#include "mpi.h"
// 
int main(int argc, char* argv[])
{
	//
	// MPI
	//
	int         rank,size;
	MPI_Status  status;
	int         tag=0;
	//
	// other
	//
	double      val;
	//
	// MPI
	//
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	//
	printf("*** STARTING %d/%d\n",rank,size);
	//
	// manager worker
	//
	if(rank==0)
	{
		//
		// sending
		//
		val = 123.0;
		printf("send 1 %f\n",val);
		MPI_Send(&val,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
		//
		val = 456.0;
		printf("send 2 %f\n",val);
		MPI_Send(&val,1,MPI_DOUBLE,2,tag,MPI_COMM_WORLD);
		//
		val = 789.0;
		printf("send 3 %f\n",val);
		MPI_Send(&val,1,MPI_DOUBLE,3,tag,MPI_COMM_WORLD);
		//
		// receiving
		//
		MPI_Recv(&val,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD,&status);
		printf("recv 1 %f\n",val);
		//
		MPI_Recv(&val,1,MPI_DOUBLE,2,tag,MPI_COMM_WORLD,&status);
		printf("recv 2 %f\n",val);
		//
		MPI_Recv(&val,1,MPI_DOUBLE,3,tag,MPI_COMM_WORLD,&status);
		printf("recv 3 %f\n",val);
	}
	else
	{
		MPI_Recv(&val,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
		//
		val = 100*val+rank;
		//
		MPI_Send(&val,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
	}
	//
	printf("--- STOPPING %d/%d\n",rank,size);
	//
	// MPI
	//
	MPI_Finalize();
	//
	return 0;
}
// 
// end of file
// 
