//*********************************************************
//
// Mehmet Ozkan
// Advanced Operating Systems
// Project I 
// MPI_Scatter Implementation
// 10/29/2018
// Instructor: Dr. Mohammed Belkhouche
//*********************************************************
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
void My_MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int numprocs, rank, size_of_send;
    MPI_Status status;
    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);
    MPI_Type_size(sendtype, &size_of_send);
    if (rank == root)
    {
		
		void *scatter_data = malloc(size_of_send); //define the scatter_data with the size_of_send
		size_of_send *= sendcount; //assign the size_of_send by multiplying with sendcount 
		
		
		 
		 
        for (int i = 0; i < numprocs; i++)
        {
            if (i != root)
            {
				memcpy(scatter_data, sendbuf, size_of_send); //copy the stored data in send_data to scatter_data 
				
                MPI_Send(scatter_data, sendcount, sendtype, i, 10, comm);// send scatter_data to the processes other than root
                
            }
            else
			{
				 memcpy(recvbuf, sendbuf, size_of_send); // if it is root, copy the data which is in sendbuf to the recvbuf
				 
			}
			 sendbuf = (char*)sendbuf + size_of_send; //assign the address of sendbuf which is equal to address of sendbuf + size_of_send
		}
		
	}
    else
    {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 10, comm, &status); // if the process is not root receive the data from root process 
    }
}


int main (int argc, char *argv[]) 
{
	
	
  
   int rank,size,i,j;
   double param[30],mine;
   int sndcnt,revcnt;
   	double t1, t2;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   revcnt=1;

   if(rank==0)
   {
       for(i=0;i<20;i++) {
	      param[i]=23.0+i;
       
	   }
	   sndcnt=1;
	  
	
   }
   t1 = MPI_Wtime();//start timer 
   My_MPI_Scatter(param,sndcnt,MPI_DOUBLE,&mine,revcnt,MPI_DOUBLE,0,MPI_COMM_WORLD);
   t2 = MPI_Wtime();//stop timer
   printf("P:%d mine is %f\n",rank,mine);//display data which is belong to the each process 
   printf("Time elapsed is %f seconds\n", t2-t1);//display time 
   MPI_Finalize();
}