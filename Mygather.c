//*********************************************************
//
// Mehmet Ozkan
// Advanced Operating Systems
// Project I 
// MPI_Gather Implementation
// 10/29/2018
// Instructor: Dr. Mohammed Belkhouche
//*********************************************************
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"






void My_MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int numprocs, rank, size_of_receive, size_of_send;
    MPI_Status status;
    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);
	MPI_Type_size(recvtype, &size_of_receive);
    MPI_Type_size(sendtype, &size_of_send);
    if (rank == root)
    {
		void *gather_data = recvbuf; //assign the address of receive buffer
		size_of_receive *= recvcount; //multiply number of receiving elements by size of recvtype and assign to the size_of_receive
        size_of_send *= sendcount;////multiply number of sending elements by size of sendtype and assign to the size_of_send
		
		
		 memcpy(gather_data, sendbuf, size_of_send); //copy the stored data in sendbuf to gather_data 
		 
        for (int i = 0; i < numprocs; i++)
        {
            if (i != root)
            {
				gather_data = (char*)gather_data + size_of_receive; //assign the address of gathering data which is equal to address of gathering data + size_of_receive
                MPI_Recv(gather_data, recvcount, recvtype, i, 10, comm, &status);//gather all data from other processes 
                
            }
            
        }

    }
    else
    {
        MPI_Send(sendbuf, sendcount, sendtype, root, 10, comm); //send the data to the root process
    }
}












int main( int argc, char **argv )
{
	
	//double time_spent = 0.0;

	//clock_t begin = clock(); //start timer 
	
    int isend, irecv[30];
    int rank, numproc;
	double t1, t2;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numproc );

    isend = rank + 1; // assign the value of rank +1 to the each process
	t1 = MPI_Wtime();// start timer
    My_MPI_Gather(&isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0, MPI_COMM_WORLD);
	t2 = MPI_Wtime();// stop timer
	
    if(rank == 0){
		
	
		for(int i=0; i<numproc; i++)
	{
    printf("Received data[%d] = %d \n", i,irecv[i]);//display gathered data
	}
	
	}
	printf("Time elapsed is %f seconds\n", t2-t1);//display time
    MPI_Finalize();
	
	
    return 0;
}