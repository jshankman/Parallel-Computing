// 
// Torbert, 18 October 2012
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
	int         j,k;
	int         val2;
	//
	// MPI
	//
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	//
	if(rank==0) // manager
	{
		val = 123.0;
		MPI_Send(&val,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
		//
		val = 456.0;
		MPI_Send(&val,1,MPI_DOUBLE,2,tag,MPI_COMM_WORLD);
		//
		val = 789.0;
		MPI_Send(&val,1,MPI_DOUBLE,3,tag,MPI_COMM_WORLD);
		//
		//
		//
		for(j=1;j<size;j++)
		{
			MPI_Recv(&val ,1,MPI_DOUBLE,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
			k=status.MPI_SOURCE;
			//
			MPI_Recv(&val2,1,MPI_INT   ,k             ,tag,MPI_COMM_WORLD,&status); // now we know
			//
			printf("recv %d %f %d\n",k,val,val2);
		}
	}
	else // workers
	{
		MPI_Recv(&val,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
		//
		val  = 100*val+rank;
		val2 = 10*rank+rank;
		//
		MPI_Send(&val ,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
		MPI_Send(&val2,1,MPI_INT   ,0,tag,MPI_COMM_WORLD);
	}
	//
	MPI_Finalize();
	//
	return 0;
}
// 
// end of file
// 
