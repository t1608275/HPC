/*****************************************************************************
  This program demonstrates non-blocking operations. This version does
  similar to mpi03, but in this version process 0 receives the message from 
  process 2 before process 0. The send from process 2 to process 0 is not 
  delayed because process 1 goes to sleep.  

  To compile:
     mpicc -o mpi04 mpi04.c
     
  To run 3 processes on this computer:
    mpirun -n 3 ./mpi04
  
  To run 3 processes on this computer:
    mpirun -H localhost,localhost,localhost ./mpi04
    
  To run on multiple computers replace localhost with hostnames or ip 
  addresses of the computers that you want to run on. In order for the program 
  to be runnable on other computers you must copy the executable and any data 
  files it needs to the other computers. If you are using relative addressing 
  to access the executable, i.e. by using ./ then the executable and resources
  need to be in the home directory 
  of the user that you will be logged in as. It could be easier to copy the 
  executable to the /tmp directory of the current computer and other computers
  in the cluster and use an invocation based on:
  
    mpirun -H localhost,ip0,ip1,ip1 /tmp/mpi04
    
  Things to note in this program:
    - MPI_Barrier causes all processes to wait at that point until all
      processes have reached that point. Here we do not allow any processes to
      finalise until all processes have reached the barrier.
    - MPI_Probe does a non-blocking check to see if messages are waiting to be 
      received. If there is nothing waiting, flag is set to 0. If there is a
      message ready to be received then flag is set to 1. In this case process
      0 finds out which process the message is coming from and proceeds to
      receive it.
       
  Dr K A Buckley, University of Wolverhampton, 2017
******************************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int size, rank;
  int received_from_process_1 = 0;
  int received_from_process_2 = 0;

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
      int flag;
      MPI_Status status;

      while(!received_from_process_1 || !received_from_process_2){
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&flag, &status);
        printf("flag = %d\n", flag);
        if(flag==1){
          printf("Process 0 knows process %d wants to send with tag %d\n",
            status.MPI_SOURCE, status.MPI_TAG);
          if(status.MPI_SOURCE == 1){
            MPI_Recv(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, 
                        MPI_STATUS_IGNORE);
            printf("Received %d from process %d\n", x, 1);
            received_from_process_1 = 1;
          }
          if(status.MPI_SOURCE == 2){
            MPI_Recv(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, 
                        MPI_STATUS_IGNORE);
            printf("Received %d from process %d\n", y, 2);
            received_from_process_2 = 1;
          }
        }           
      }
    } else {
      if(rank == 2){
        usleep(5000000);
        int number = rank + 10;
        printf("process 2 ready to send\n");
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      } 
      if(rank == 1){
        usleep(3000000);
        int number = rank + 10;
        printf("process 1 ready to send\n");
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      } 
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize(); 

  return 0;
}
