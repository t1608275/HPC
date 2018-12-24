/****************************************************************************
  This program outputs x and y coordinates found by passing x through the 
  function f. You should redirect the output into a file and plot a graph
  to make sure that it looks like the graph in lecture notes.

  Compile with:

    cc -o optimisation_0 optimisation_0.c -lm

  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

#include <stdio.h>
#include <math.h>

double f(double x) {
  return cos(x) + 2;
}

int main() {
  int i;
  double x, y;

  for(i=0;i<600;i++) {
    x = i / 100.0;
    y = f(x);
    printf("%6.4lf %6.4lf\n", x, y);
  }

  return 0;
}
