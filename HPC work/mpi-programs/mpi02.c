/*****************************************************************************
  This program demonstrates how to send a message from the process with rank
  0 to other processes. The other processes execute a blocking receive 
  instruction. This causes them to wait for a message before proceeding to 
  print out the data that was received. 

  To compile:
     mpicc -o mpi02 mpi02.c
     
  To run 3 processes on this computer:
    mpirun -n 3 ./mpi02
  
  To run 3 processes on this computer:
    mpirun -H localhost,localhost,localhost ./mpi02
    
  To run on multiple computers replace localhost with hostnames or ip 
  addresses of the computers that you want to run on. In order for the program 
  to be runnable on other computers you must copy the executable and any data 
  files it needs to the other computers. If you are using relative addressing 
  to access the executable, i.e. by using ./ then the executable and resources 
  need to be in the home directory of the user that you will be logged in as.
  It could be easier to copy the executable to the /tmp directory of the 
  current computer and other computers in the cluster and use an invocation 
  based on:
    mpirun -H localhost,ip0,ip1,ip1 /tmp/mpi02
    
  Things to note in this program:
    - Process 0 sends x to process 1 and y to processes 2. "1, MPI_INT" 
      indicates that the message consists of one integer. A message tag can 
      be used to indicate the type of message to be sent. In this case we only
      have one kind of message so both sends use the value 0 for the tag. See:
      https://www.open-mpi.org/doc/current/man3/MPI_Send.3.php
    - Processes other than rank 0 wait to receive a message using MPI_Recv. 
      The "0, 0" indicates that the message is expected from process 0 and 
      should have the tag 0. The result is stored in the number variable. See:
      https://www.open-mpi.org/doc/current/man3/MPI_Recv.3.php
       
  Dr K A Buckley, University of Wolverhampton, 2017
*****************************************************************************/

#include <stdio.h>
#include <mpi.h>

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
      int x = 9;
      int y = 17;
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } else {
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
      printf("Process %d received %d\n", rank, number); 
    }
  }
  MPI_Finalize(); 

  return 0;
}
