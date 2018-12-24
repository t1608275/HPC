#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o multithreading multithreading.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./multithreading > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$0G24VuNaA9ApVG4z8LkI/OOr9a54nBfzgQjbebhqBZxMHNg0HiYYf1Lx/HcGg6q1nnOSArPtZYbGy7yc5V.wP/",
  "$6$KB$VDUCASt5S88l82JzexhKDQLeUJ5zfxr16VhlVwNOs0YLiLYDciLDmN3QYAE80UIzfryYmpR.NFmbZvAGNoaHW.",
  "$6$KB$0n1YjoLnJBuAdeBsYFW3fpZzMPP8xycQbEj35GvoerMnEkWIAKnbUBAb70awv5tfHylWkVzcwzHUNy/7l7I1c/",
  "$6$KB$HKffNNiGzngqYueF89z3gwWZMg.xUBIz/00QSCbgwKtRHmwUbZX6jTH4VUAg3L3skaO8qtNf5LE7WP39jQ7ZJ0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

typedef struct arguments_t {
  int offset;
  int n_threads;
} arguments_t;

void factorise() {
  pthread_t t1, t2;
  arguments_t t1_arguments;
  t1_arguments.offset = 0;
  t1_arguments.n_threads = 2;
  
  arguments_t t2_arguments;
  t2_arguments.offset = 1;
  t2_arguments.n_threads = 2;

  void *crack();
  void *crack2();
  
  pthread_create(&t1, NULL, crack, &t1_arguments);
  pthread_create(&t2, NULL, crack2, &t2_arguments);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void *crack(char *salt_and_encrypted){
  int x, y, i, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
     for(i='A'; i<='Z'; i++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%c%02d", x, y, i, z); 
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
}
  printf("%d solutions explored\n", count);
}

void *crack2(char *salt_and_encrypted){
  int x2, y2, i2, z2;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc2;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
 for(x2='N'; x2<='Z'; x2++){
    for(y2='A'; y2<='Z'; y2++){
	for(i2='A'; i2<='Z'; i2++){
      for(z2=0; z2<=99; z2++){
        sprintf(plain, "%c%c%c%02d", x2, y2, i2, z2);
        enc2 = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc2) == 0){
          printf("#%-8d%s %s\n", count, plain, enc2);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc2);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", count);
}


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

int main() {
  struct timespec start, finish;   
  long long int time_elapsed;

  int i;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
    for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
