/*****************************************************************************
  To compile:
     mpicc -o mpi01 mpi01.c
     
  To run 4 processes on this computer:
    mpirun -n 4 ./mpi01
  
  To run 4 processes on this computer:
    mpirun -H localhost,localhost,localhost,localhost ./mp01
    
  To run on multiple computers replace localhost with hostnames or ip 
  addresses of the computers that you want to run on. In order for the program 
  to be runnable on  other computers you must copy the executable and any data 
  files it needs to the other computers. If you are using relative addressing 
  to access the executable, i.e. by using ./ then the executable and resources 
  need to be in the home directory  of the user that you will be logged in as. 
  It could be easier to copy the executable to 
  the /tmp directory of the current computer and other computers in the 
  cluster and use an invocation based on:
  
    mpirun -H localhost,ip0,ip1,ip1 /tmp/mpi01
    
  In this case the program will be run twice on the computer with ip address 
  ip1 and once on the local computer and once on the computer at address ip0.
  
  Things to note in this program:
    - MPI_Init must be used to initialise MPI. The OpenMPI implementation does
      not take parameters, hence why NULL is passed. See 
      https://www.open-mpi.org/doc/current/man3/MPI_Init.3.php
    - MPI_Comm_size finds out how many processes will be run, i.e. the number 
      of parameters to -H switch to mpirun the integer following the -n switch.
      For the purposes of this module the first parameter will always be 
      MPI_COMM_WORLD as we will not be using communicator groups. See
      https://www.open-mpi.org/doc/current/man3/MPI_Comm_size.3.php
    - MPI_Comm_rank gets the process id, rank. The process with rank. 0 is 
      thought of as the master process and is used to control the overall 
      processing done by the others.
    - MPI_Finalize closes down MPI. All processes must call this to close down
      MPI. You cannot make any more calls to the MPI API after this has been 
      called. If this is not called for some reason (like you return
      from main or call exit, then mpirun will complain. Also, there will be a
      problem if it is called muliple times from a processs.
      
  Dr K A Buckley, University of Wolverhampton, 2017
*****************************************************************************/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("I am %d of %d\n", rank, size);
  MPI_Finalize();

  return 0;
}
