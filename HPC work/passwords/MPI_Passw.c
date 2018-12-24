#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

/******************************************************************************

  Compile with:
    cc -o MPI_Passw M.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;
int size, rank;
char buffer[4];


char *encrypted_passwords[] = {
  "$6$KB$0G24VuNaA9ApVG4z8LkI/OOr9a54nBfzgQjbebhqBZxMHNg0HiYYf1Lx/HcGg6q1nnOSArPtZYbGy7yc5V.wP/",
  "$6$KB$VDUCASt5S88l82JzexhKDQLeUJ5zfxr16VhlVwNOs0YLiLYDciLDmN3QYAE80UIzfryYmpR.NFmbZvAGNoaHW.",
  "$6$KB$0n1YjoLnJBuAdeBsYFW3fpZzMPP8xycQbEj35GvoerMnEkWIAKnbUBAb70awv5tfHylWkVzcwzHUNy/7l7I1c/",
  "$6$KB$HKffNNiGzngqYueF89z3gwWZMg.xUBIz/00QSCbgwKtRHmwUbZX6jTH4VUAg3L3skaO8qtNf5LE7WP39jQ7ZJ0"
};



void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far
   char xStart,yStart, xEnd, yEnd;
 
   if (rank ==1) {
	xStart='A';
	yStart='A';
	xEnd='M';
	yEnd='Z';
	}

	if (rank ==2) {
	xStart='N';
	yStart='A';
	xEnd='Z';
	yEnd='Z';
	}
  substr(salt, salt_and_encrypted, 0, 6);

  for(x=xStart; x<=xEnd; x++){
    for(y=yStart; y<=yEnd; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference) {
	long long int ds = finish->tv_sec - start->tv_sec;
	long long int dn = finish->tv_nsec - start->tv_nsec;

	if (dn < 0) {
		ds--;
		dn += 1000000000;
	}

	*difference = ds * 1000000000 + dn;
	return !(*difference > 0);
}

int main(int argc, char *argv[]){
MPI_Status status;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  struct timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int i;
  if (rank == 0) {
	MPI_Finalize();
  }
  else 
	{
 	for (i = 0; i < n_passwords; i++) {
		  crack(encrypted_passwords[i]);
	}
  }

  MPI_Finalize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elasped was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));

  return 0;
}
