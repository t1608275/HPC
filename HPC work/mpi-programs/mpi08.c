/*****************************************************************************
  This program demonstrates how to send a string. C is not an object oriented
  language so unlike object oriented langages strings in C cannot have methods.
  We must therefore uses the functions declared in string.h to manipulate
  strings. In this example the length of a string must be determined and sent 
  to the recipient before the string can be sent. This is so that the 
  recipient knows how many bytes to receive. Note that we need to add 1 to 
  the string length to account for the extra byte used as an end of string
  marker.

  To compile:
     mpicc -o mpi08 mpi08.c
     
  To run 2 processes on this computer:
    mpirun -n 2 ./mpi08
  
  To run 2 processes on this computer:
    mpirun -H localhost,localhost ./mpi08
    
  To run on multiple computers replace localhost with hostnames or ip 
  addresses of the computers that you want to run on. In order for the program 
  to be runnable on other computers you must copy the executable and any data 
  files it needs to the other computers. If you are using relative addressing 
  to access the executable, i.e. by using ./ then the executable and resources 
  need to be in the home directory of the user that you will be logged in as.
  It could be easier to copy the executable to the /tmp directory of the 
  current computer and other computers in the cluster and use an invocation 
  based on:
    mpirun -H localhost,ip0 /tmp/mpi08
  
  Dr K A Buckley, University of Wolverhampton, 2017
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 2) {
    if(rank == 0) {
      printf("This program needs to run on exactly 2 processes\n");
    }
  } else {
    if(rank ==0){
      char *text = "Kevan loves HPC";
      int length = strlen(text);
      MPI_Send(&length, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(text, length+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else {
      int length;
      char buffer[100]; // assume string length is less than 100
      MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("string to be recieived has %d characters\n", length);
      MPI_Recv(buffer, length+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process %d received %s\n", rank, buffer); 
    }
  }
  MPI_Finalize(); 

  return 0;
}
