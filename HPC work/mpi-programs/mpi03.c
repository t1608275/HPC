/*****************************************************************************
  This program demonstrates how to send a message from the process with rank
  0 to other processes. The other processes execute a blocking receive 
  instruction. This causes them to wait for a message before proceeding to 
  print out the data that was received. Process 0 wants to receieve from 
  process 1 before process 2. When process 1 goes to sleep blocking causes
  the send from process 2 to process 0 to be delayed by 5 seconds. 

  To compile:
     mpicc -o mpi03 mpi03.c
     
  To run 3 processes on this computer:
    mpirun -n 3 ./mpi03
  
  To run 3 processes on this computer:
    mpirun -H localhost,localhost,localhost ./mpi03
    
  To run on multiple computers replace localhost with hostnames or ip 
  addresses of the computers that you want to run on. In order for the program 
  to be runnable on other computers you must copy the executable and any data 
  files it needs to the  other computers. If you are using relative addressing 
  to access the executable, i.e. by using ./ then the executable and resources 
  need to be in the home directory  of the user that you will be logged in as. 
  It could be easier to copy the executable to the /tmp directory of the 
  current computer and other computers in the cluster and  use an invocation 
  based on:
   
    mpirun -H localhost,ip0,ip1,ip1 /tmp/mpi03
    
       
  Dr K A Buckley, University of Wolverhampton, 2017
*****************************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){
      int x;
      int y;
      MPI_Recv(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
      printf("Received %d from process %d\n", x, 1); 
      MPI_Recv(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
      printf("Received %d from process %d\n", y, 2); 
    } else {
      if(rank == 1){
        usleep(5000000);
      }
      int number = rank + 10;
      MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize(); 

  return 0;
}
