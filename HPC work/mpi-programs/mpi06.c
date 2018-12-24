/*****************************************************************************
  This program demonstrates how to use MPI_Bcast. This is a broadcast 
  operation. It is not the same send and messages that are sent by
  broadcast cannot be read by a receive or probe. 
  
  Broacast sends a message to all processes from process 0. This is a blocking
  operation, so none of the other processes can advance past the call to 
  broadcast until they receive a broadcast message.
   
  In this application process 0 immediately goes to sleep for 5 seconds. 
  Concurrent to that all other processes call MPI_Bcast which forces them to
  wait until they receive a broadcast. When process 0 finishes its sleep it
  calls broadcast, which releases all other processes to move on.

  To compile:
     mpicc -o mpi06 mpi06.c
     
  To run on this computer and two other computers with address ip1, ip2
    mpirun -H localhost,ip1,ip2 ./mpi06
       
  Dr K A Buckley, University of Wolverhampton, 2017
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("I am %d of %d\n", rank, size);

  if(rank==0){   
    printf("process %d is about to have a sleep\n", rank);
    usleep(5000000);
    printf("process %d has finished sleeping and about to start broadcasting\n", 
           rank);
    int message = 19;
    MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("process %d has finished broadcasting\n", rank);
  } else {
     printf("process %d is about to receive a broadcast\n", rank);
     int message;
     MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
     printf("process %d has received %d\n", rank, message);
  }
  
  printf("process %d held at barrier\n", rank);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize(); 
  printf("process %d has finalised\n", rank);
  return 0;
}



