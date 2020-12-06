/**************************************************************************
  intTest.c - for students
  This program demonstrates the speed advantages of integer versus floating
  point math.
 12/28/2015 - R. Repka  Initial version
 10/12/2016 - R. Repka  Switched to PRINT_RTIMER() macro
 03/25/2018 - R. Repka  Switched to timing macros
 11/18/2019 - R. Repka  Polynomial comment error
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/****************************************************************************
  Use this to define the number of iterations in your performance measurements
  ----------- You will need to adjust this number  --------
****************************************************************************/
#define ITERATIONS   (500)

/* Optional verbose flag, compile with -DVERBOSE or not */
#ifdef VERBOSE
   #undef VERBOSE
   #define VERBOSE (1)
#else
   #define VERBOSE (0)
#endif

/****************************************************************************
  Remember to set the QN value in rootfinding.h
****************************************************************************/

/************************************************************************
   Simple code to test Qn performance vs floating point
************************************************************************/
int main(int argc, char* argv[])
    {
    double Result;          /* The result from floating point calculation   */
    long  iResult;          /* The result from floating point calculation   */

    /* The bisection guesses and tolerance value                        */
    double a = -25.0;
    double b = 25.0;
    double tol= 0.000001;

    int Qn1 = 0;
    int Qn2 = 0, Qn3 = 0;

    /* You many need to modify the timing code for your macro implementation! */
    DECLARE_TIMER(timer);
    DECLARE_REPEAT_VAR(tloop)

    /************************************************************************
     Run the Qn format test
    ************************************************************************/

    //convert guesses to Fixed format

    Qn1 = FLOAT_TO_FIX(a);
    Qn2 = FLOAT_TO_FIX(b);
    Qn3 = FLOAT_TO_FIX(tol);

    /* You many need to modify the timing code for your macro implementation! */
    START_TIMER(timer);
    BEGIN_REPEAT_TIMING(ITERATIONS,tloop)
         iResult = ibisection(&iPoly, Qn1, Qn2, Qn3, VERBOSE);
    END_REPEAT_TIMING
    STOP_TIMER(timer);

    if(INAN == iResult)
        {
        fprintf(stderr, "Qn %d bisection couldn't find a root. \n", QN);
        exit(99);
        }
    else
        {
        fprintf(stderr, "Found the root %f using Qn %d bisection.\n",
                        FIX_TO_FLOAT(iResult), QN);
        }
    PRINT_TIMER(timer);
    PRINT_RTIMER(timer, ITERATIONS);

    /************************************************************************
     Run the floating point format test
    ************************************************************************/
    RESET_TIMER(timer);
    START_TIMER(timer);
    BEGIN_REPEAT_TIMING(ITERATIONS,tloop)
        Result = bisection(&rPoly, a, b, tol, VERBOSE);
    END_REPEAT_TIMING
    STOP_TIMER(timer);

    if(NAN == Result)
        {
        fprintf(stderr, "Float bisection couldn't find a root. \n");
        exit(99);
        }
    else
        {
        fprintf(stderr, "Found the root %f using float bisection.\n",
                        Result);
        }

    PRINT_TIMER(timer);
    PRINT_RTIMER(timer, ITERATIONS);

    /* Display the percentage error */
    Result = fabs(Result - FIX_TO_FLOAT(iResult))/100.0;
    fprintf(stderr, "Qn %d error= %f\n", QN, Result);
    return 0;
    } /* End main */



/******************************************************************************
 Purpose: Finds a root of scalar, non-linear function f using the integer
 bisection  method. a and b make up the initial bracket to start bisecting from.
 Only implement this after you have the floating point function implemented

 Where: ifunc1arg f - INTEGER function whose root is to be determined
                      must take a single argument of type long and return
        long a      - initial Qn root bracket guess
        long b      - initial Qn root bracket guess
        long atol   - absolute Qn error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: long - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN
******************************************************************************/
long ibisection(ifunc1arg f, long a, long b, long atol, int verb)
{

	long c;
	long fc;
	int i=0;
        long two = FLOAT_TO_FIX(2.0);

//	printf("QN1 is %ld QN2 is %ld Qn3 is %ld\n", a, b, atol);

	if((abs(a-b) < atol) || SIGN(f(a))==SIGN(f(b))){
		fprintf(stdout, "A and B do not center a root \n");
		return(INAN);
	}

	//limit attempt number
	while(i < 1000)
	{
		//calc midpoint, evaluate it
		//c = QN_DIVIDE((a + b), 2);
		c = QN_DIVIDE((a+b), two);
		fc = f(c);

		//did we find root
		//this if statement checks if it worked
		if(abs(fc) <= atol)
		{
			return c;
		}

		//did we find it - check range
		if( abs(QN_DIVIDE((a+b),two)) <= atol)
		{
			return c;
		}

		if(verb)
		{
			fprintf(stdout, "iter:%d	a:%f	b:%f	x:%f	err:%f\n", i, FIX_TO_FLOAT(a), FIX_TO_FLOAT(b), FIX_TO_FLOAT(c), fabs(FIX_TO_FLOAT(b-c)));
		}

		if(SIGN(f(a)) == SIGN(fc))
		{
			a = c;
		}
		else{
			b = c;
		}
		i++;
	}
	fprintf(stdout, "Bisection could not find a root with %d iterations", 1000);
	return(INAN);
}

/*---------------------------------------------------------------------------
  This function implements the real polynomial equation to be solved.

  Where:   double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double rPoly(double x){
    //x**3-.0001x**2-676X+.0676
    return(x* (x*(x-.0001) - 676.0) + .0676);
}


/*---------------------------------------------------------------------------
  This function implements the integer polynomial equation to be solved.

  Where:    long x - the value to evaluate
  Returns: long - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
//use MULT_FIX()
//do similar to rPoly
long iPoly(long x){

    long b = 0, c= 0;
    c = FLOAT_TO_FIX(676.0);

    //(x*(x - .0001)) - 676
    b = x - FLOAT_TO_FIX(.0001);
    b = MUL_FIX(b, x);
    b -= c;
    b = MUL_FIX(b,x);

    //adds the final .0676
    c = FLOAT_TO_FIX(.0676);
    b += c;
    return (b);
}
