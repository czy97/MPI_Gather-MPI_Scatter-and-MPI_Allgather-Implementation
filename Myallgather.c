//*********************************************************
//
// Mehmet Ozkan
// Advanced Operating Systems
// Project I 
// MPI_Allgather Implementation
// 10/29/2018
// Instructor: Dr. Mohammed Belkhouche
//*********************************************************



#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
void My_MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int numprocs, rank, size_of_receive, size_of_send;
    MPI_Status status;
    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);
	MPI_Type_size(recvtype, &size_of_receive);
    MPI_Type_size(sendtype, &size_of_send);
    if (rank == 0)
    {
		void *all_gather_data = recvbuf; //assign the address of receive buffer
		size_of_receive *= recvcount; //multiply numproc of receiving elements by size of recvtype and assign to the recvtype
        size_of_send *= sendcount;////multiply numproc of sending elements by size of sendtype and assign to the recvtype
		
		
		 memcpy(all_gather_data, sendbuf, size_of_send); //copy the stored data in sendbuf to all_gather_data 
		 
        for (int i = 1; i < numprocs; i++)
        {
       
            
				all_gather_data = (char*)all_gather_data + size_of_receive; //assign the address of gathering data which is equal to address of all_gather data + size_of_receive
                MPI_Recv(all_gather_data, recvcount, recvtype, i, 10, comm, &status);//all_gather all data from other processes 
        }        
            
            for (int i = 1; i < numprocs; i++) 
		{
            MPI_Send(recvbuf,numprocs*recvcount,recvtype,i,10,comm); //send the all_gather_data to other processes 
        }
    }

    
    else
    {
        MPI_Send(sendbuf, sendcount, sendtype, 0, 10, comm); //if the rank is different than 0 send the data to process 0
		MPI_Recv(recvbuf, recvcount*numprocs, recvtype, 0, 10, comm, &status);//if the rank is different than 0, receive all_gather_data
    }
}



int main(int argc, char **argv)
{ 
 
 int rank, numproc;
 int *data;
 int *allgather_data;
 int block_size =3;
 double t1, t2;
 
 
 MPI_Init(&argc, &argv); 
 MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
 MPI_Comm_size(MPI_COMM_WORLD, &numproc);

 allgather_data = malloc(sizeof(int)*block_size);
 
 //Store the data for each process such as P0 = 0 0 0 , P1 = 1 1 1, P2 = 2 2 2  etc. 
 for (int i = 0; i < block_size; ++i)
 {
	allgather_data[i] = rank * rank;
	data = malloc(numproc*sizeof(int)*block_size);
 }
 
 t1 = MPI_Wtime();//start timer 
 My_MPI_Allgather(allgather_data, block_size, MPI_INT, data, block_size, MPI_INT, MPI_COMM_WORLD);
 t2 = MPI_Wtime();//stop timer 
  
 if(rank==0){
  
 for (int i = 0 ; i < block_size*numproc; i++) // display the all_gather_data
 {
 printf("%d ", data[i]);
 
 }
 printf("\n"); 
 }
 printf("Time elapsed is %f seconds\n", t2-t1); // display the time 
 MPI_Finalize(); 
 
	
 return 0; 
 
} 
