2/*---------------------------------------------------------------------------
  QN code testing and verification
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>

#define LOOP_DELAY (1<<27)

// QN math functions which pass the number of bits "n"
#define Qn_MULTIPLY(A,B,n) ((A>>(n/2))*(B >>(n-(n/2))))
#define Qn_DIVIDE(A,B,n) ((int) (((long int)(A)<<n)/(B) ))

// QN math and printing functions
void  printBinary(unsigned num, short N);
int  floatToFixed(double x, int n);
float fixedToFloat(int x, int n);

/*---------------------------------------------------------------------------
  QN testing framework
---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
   // QN testing variables, add more as necessary
   double fnum1;
   int  qnum1;

   //------------------------------------------------------------------------
   // Print data on sizes
   //------------------------------------------------------------------------
   printf("Integers are %ld bits long\n", 8*sizeof(int));

   //------------------------------------------------------------------------
   // Prove floating point has limits
   //------------------------------------------------------------------------
   printf("epsilon(double) is: %.20E\n",(double)DBL_EPSILON);
   printf("The result of adding 10.0 and DBL_EPSILON is: %.2E\n", (double)10.0 + (double)DBL_EPSILON);

   //------------------------------------------------------------------------
   // Examine the conversion utilities
   //------------------------------------------------------------------------
   printf("\nConversion test\n");

   //converts 0.0 to Qn format then back to float
   qnum1 = floatToFixed(0.0, 0);
   printBinary(qnum1, 8);
   fnum1 = fixedToFloat(qnum1, 8);
   printBinary(fnum1, 8);
   printf("converted 0.0 to Qn0 00000000 then back to float f\n");

   //convert 12.25 to Qn then back
   qnum1 = floatToFixed(12.25, 3);
   printBinary(qnum1, 8);
   fnum1 = fixedToFloat(qnum1, 3);
   printBinary(fnum1, 8);
   printf("converted 12.25 to Qn3 01100010 then back to float f\n");

   //convert 12.0625 to Q3 then back
   qnum1 = floatToFixed(12.0625, 3);
   printBinary(qnum1, 8);
   fnum1 = fixedToFloat(qnum1, 3);
   printBinary(fnum1, 8);
   printf("converted 12.0625 to Qn3 01100000 then back to float \n" );

   //convert 12.0625 to Q4 then back
   qnum1 = floatToFixed(12.0625, 4);
   printBinary(qnum1, 8);
   fnum1 = fixedToFloat(qnum1, 4);
   printBinary(fnum1, 8);
   printf("converted 12.0625 to Qn4 11000001 then back to float \n");

return(0);
}


/*---------------------------------------------------------------------------
  This prints a number in character binary bracketed by [..]

  Where: unsigned num - number to print
         short N      - number of bits to print

  Returns: nothing
  Error handling: none
----------------------------------------------------------------------------*/
void printBinary(unsigned num, short N) {
   unsigned i;
   printf("\t[");
   for (i = 1 << (N-1); i > 0; i = i >> 1) {
      (num & i)? printf("1"): printf("0");
    }
   printf("]\n");
   return;
}


/*---------------------------------------------------------------------------
  This convert from Float to FP(Qn)

  Where: double x  - number to convert
         int qnval - number of bits to use

  Returns: int    - QN encoded number
  Error handling: none
----------------------------------------------------------------------------*/
int floatToFixed(double x, int qnval) {
   return( x * (double)(1 << qnval) );
}

/*---------------------------------------------------------------------------
  This converts from FP(Qn) to Float

  Where: int x     - number to convert
         int qnval - number of bits to use

  Returns: float   - converted number
  Error handling: none
----------------------------------------------------------------------------*/
float fixedToFloat(int x, int qnval) {
   return((double)x / (double) (1 << qnval));
}

