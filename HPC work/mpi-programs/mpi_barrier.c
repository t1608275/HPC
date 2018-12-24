#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int i;
  useconds_t sleeplen;
  sleeplen = (size - rank) * 1000000;
  printf("Process %d about to sleep for %d seconds\n", rank, sleeplen); 
  usleep(sleeplen);
  printf("Process %d woken up\n", rank); 
  MPI_Barrier(MPI_COMM_WORLD);
  printf("Process %d passed MPI_Barrier\n", rank); 
  MPI_Finalize(); 

  return 0;
}
