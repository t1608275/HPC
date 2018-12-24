/*****************************************************************************
  This program finds groups of four numbers that when multiplied together
  equal 411530880661. The number of threads to use is specified near the
  start of the code. Node that because the numbers are so large long integers 
  are required in this example. There are 2 main parts to the program as 
  follows.

  Process 0 code:
    - Listens for an incoming message.
    - When a message is received, it should be a notification from a different
      process that a solution has been found.
    - The details of the solution are then received.
    - The done variable is set so that the main loop may exit.
    - All processes are then sent a message to indicate that a solution has 
      been found and they can stop the search. 

  Code for other processes:
    - 3 nested loops are used to search for a solution.
    - Just inside the outer loop there is a check to see if a message is ready
      to be received. If one is, a solution must have been found so this 
      process should stop.
    - If the condition in the innermost loop identifies that a solution has
      been found the the solution is packaged into an array and is sent to
      process 0.   
    - It is possible that multiple solutions may be found as processes 
      continue executing their outer until the next call to iprobe to check
      for an incoming message. However, process 0 will only ever receive one 
      solution. It is possible to call iprobe more frequently but that will 
      have an impact on pderforance.

  To compile:
     mpicc -o mpi07 mpi07.c -lrt
     
  To run on this computer and two other computers with address ip1, ip2
    mpirun -H localhost,ip1,ip2 ./mpi07
    
  
  Things to note in this program:
    - MPI_Iprobe checks to see is a message is waiting to be received. The 
      flag variable stores 1 if a message is waiting, otherwise 0.
       
  Dr K A Buckley, University of Wolverhampton, 2017
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

#define goal 411530880661L

struct timespec start, finish;

int time_difference(struct timespec *start, struct timespec *finish, 
                              long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char** argv) {
  int size, rank;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("I am %d of %d\n", rank, size);

  if(rank ==0){   
    clock_gettime(CLOCK_MONOTONIC, &start);
   
    long solution[4];
    MPI_Status status;
    MPI_Request request[65]; // Assume there are at most 65 processes.
                             // Use malloc for more.
    int done = 0;
    int i;

    while(!done){
      MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &done, &status);

      if(done==1) {
        MPI_Recv(&solution, 4, MPI_LONG, status.MPI_SOURCE, status.MPI_TAG, 
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);        

        printf("received solution (%ld, %ld, %ld, %ld) from process %d\n", 
               solution[0], solution[1], solution[2], solution[3],
               status.MPI_SOURCE);

        for(i=1;i<size;i++){
          if(i!=status.MPI_SOURCE){
            printf("sending done to process %d\n", i);
            MPI_Isend(&done, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]);
          }   
        }
      }          
    }
  } else {
    long a, b, c, d;    
    int done = 0;
    MPI_Status status;
    MPI_Request request;
    long solution[4];
    
    for(a=rank;a<1000 && !done;a+=size-1){
      printf("process %d exploring first factor = %ld\n", rank, a);
      for(b=0;b<1000 && !done;b++){
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &done, 
                   &status);
        if(done){
          printf("process %d has received done signal\n", rank);
          break;
        }
        
        for(c=0;c<1000 && !done;c++){        
          for(d=0;d<1000 && !done;d++){        
            if(a*b*c*d == goal){
              printf("process %d has found solution %ld, %ld, %ld, %ld\n", 
                     rank, a, b, c, d);
              solution[0] = a;
              solution[1] = b;
              solution[2] = c;
              solution[3] = d;
              MPI_Send(&solution, 4, MPI_LONG, 0, 0, MPI_COMM_WORLD);
                 
              printf("status ");
              done=1;
            }
          }
        }
      }
    }
    printf("process %d is about to finish\n", rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize(); 

  if(rank == 0){
    long long int time_elapsed;
    clock_gettime(CLOCK_MONOTONIC, &finish);
    time_difference(&start, &finish, &time_elapsed);
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
           (time_elapsed/1.0e9)); 
  }
  return 0;
}



