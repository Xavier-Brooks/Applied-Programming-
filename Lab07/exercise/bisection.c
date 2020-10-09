/*-----------------------------------------------------------------------
  Simple code to test a bisection solver.
  student copy
-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "rootfinding.h"
double bisection(func1arg f, double a, double b, double atol, int verb);

#define BI_LIMIT   (1000)
#define VERBOSE     (1)


/*-----------------------------------------------------------------------
  The function contains the equation to solve.

  Where:   double x  - the point to evaluate
  Returns: double    - the value of the function at the point.
  Error Handling: none
-----------------------------------------------------------------------*/
double func(double x) {
	double ans;
	double temp1 = x*x;
	double temp2 = x*x*x;
	ans = 0- 52.2*x - .75*temp1 + .02*temp2 + 1909;
	return ans;
}

/*-----------------------------------------------------------------------
  Simple main() to run the bisection code.
  Roots: 35.687256, 52.632141, -50.809979
-----------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
   // The initial guesses
   double a   = 0.0;
   double b   = 50.0;
   double tol = 0.01;
   double result;

   // Call the bisection solver
   result = bisection(&func, a, b, tol, VERBOSE);
   printf("Bisection returned root: %f\n", result);
   result = bisection(&func, 36, 75, tol, VERBOSE);
   printf("Bisection returned root: %f\n", result);
   result = bisection(&func, -60, 0, tol, VERBOSE);
   printf("Bisection returned root: %f\n", result);
   return 0;
}


/******************************************************************************
 Purpose: Finds a root of scalar, non-linear function f using the bisection
 method. a and b make up the initial bracket to start bisecting from.

 Where: func1arg f  - function whose root is to be determined
                      must take a single argument of type double and return
        double a    - initial root bracket guess
        double b    - initial root bracket guess
        double atol - absolute error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN
******************************************************************************/
double bisection(func1arg f, double a, double b, double atol, int verb)
{
    double c; /* midpoint */
    double fc; /* the value at the midpoint */
    int i=0; /* iterator */
    
    if ((fabs(a-b) < atol) ||  SIGN(f(a))==SIGN(f(b))){
        fprintf(stdout, "A and B do not center a root\n");
        return(NAN);
    }

    /* Limit the number of attempts */
    while(i < BI_LIMIT) {
        /* Calculate the mid point, evaluate it */
        c = (a+b)/2.0;
        fc = f(c);
        
        /* Did we find a root? - check the function */
        if (fabs(fc) <= atol) {
            return c;
        }
        
        /* Did we find a root? - check the range */
        if (fabs(a-b)/2.0 <= atol) {
           return(c);
        }
        
        if(verb) {
            fprintf(stdout, "iter:%d	a:%f	b:%f	x:%f	err:%f\n",
                    i, a, b, c, fabs(b-c));
        }
        
        /* Pick the next bracket */
        if (SIGN(f(a))==SIGN(fc)) {
            a = c;
        }
        else {
            b = c;
        }
        i++;
    }
    fprintf(stdout, "Bisection could not find a root with %d iterations\n", BI_LIMIT);
    return(NAN);
}
