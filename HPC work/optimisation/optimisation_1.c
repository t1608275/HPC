/****************************************************************************
  This program performs a simple downhill gradient descent search of a 
  simple function. It uses a fixed step size. Try running it with step size
  of 0.25, then with step sizes of 0.1 and 0.01. Note the effect this has on
  running time and accuracy. Note that the solution to this problem is
  equal to the mathematical constant pi.

  The program uses variables a, b, c to store the current best guess (b) and
  two values a step away from b. Try plotting b against f(b) and check that
  the search procedes in the anticipated way.

  Have a think about how you could make this program multithreaded, discuss
  it with your tutor and have a go implementing it. By doing this you will be 
  working on similar theory to that needed for a portfolio task, but on a 
  simpler problem.

  Try your code on some different functions and different starting guesses.

  Compile with:

    cc -o optimisation_1 optimisation_1.c -lm

  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

#include <stdio.h>
#include <math.h>

#define step_size 0.25
#define initial_guess 1.0

double f(double x) {
  return cos(x) + 2;
}

int main() {
  double a = initial_guess - step_size;
  double b = initial_guess; 
  double c = initial_guess + step_size;
  double fa = f(a);
  double fb = f(b);
  double fc = f(c);

  while(fb > fa || fb > fc) {
    if(fa < fb) {
      b = a;
      fb = fa;
    } else {
      b = c;
      fb = fc;
    }
    a = b - step_size;
    c = b + step_size;
    fa = f(a);
    fc = f(c);
    printf("%6.4lf %6.4lf\n", b, fb);
  } 
  printf("solution is x=%6.4lf with f(x)=%6.4lf\n", b, fb);

  return 0;
}
