#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int message = 0;

  if(rank==0){   
    printf("process %d is about to have a sleep\n", rank);
    usleep(1000000);
    printf("process %d has finished sleeping and about to start broadcasting\n", rank);
    message = 42;
  } else {
    printf("process %d is about to receive broadcast\n", rank);
  }

  MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank==0){   
    printf("process %d has finished broadcasting\n", rank);
  } else {
     printf("process %d has received %d\n", rank, message);
  }
  MPI_Finalize(); 
  return 0;
}

