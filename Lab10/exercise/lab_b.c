/*---------------------------------------------------------------------------
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
#define Qn_MULTIPLY(A,B,n) ((A>>(n/2))*(B >> (n-(n/2))))
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
   double fnum1, fnum2, fnum3;
   int  zero, qnum1, qnum2, qnum3, qnum4;
   double ftemp1, ftemp2;
   clock_t  time1, time2;
   unsigned i;

   //------------------------------------------------------------------------
   // Examine the division utilities
   //------------------------------------------------------------------------
   printf("\nDivision test\n");
   fnum1 = -.0625;
   qnum1 = floatToFixed(fnum1, 18);	//seperate qnum for float divid testing
   qnum2 = qnum1; 			//seperate qnum for Qn_DIVIDE testing
   zero = 0;				/*flag to set for when one of the qnums
					  is 0*/

   qnum3 = floatToFixed(2.0, 18);

   while(zero == 0){
   	fnum1 /= 2.0;			//loop should divide float by 2.0?
        qnum1 = qnum1 / 2;		/*loop should divide one of the qnum
					  by integer 2*/

	qnum2 = Qn_DIVIDE(qnum2, qnum3,18);
        if((qnum1 == 0) || (qnum2 == 0)){
    		zero = 1;
        }

   	ftemp1 = fixedToFloat(qnum1, 18);
	ftemp2 = fixedToFloat(qnum2, 18);

	printf("fnum		qnum1(dec)	qnum2(dec)	qnum1(float)	qnum2(float)\n");
   	printf("%.6f		%d		%d		%.6f		%.6f\n",fnum1, qnum1, qnum2, ftemp1, ftemp2);

   }


//   prints the absolute value of the floating points
//   printf("%.6f		%d		%d		%.6f		%.6f\n",fabs(fnum1), abs(qnum1), abs(qnum2), fabs(ftemp1), fabs(ftemp2));

   //------------------------------------------------------------------------
   // Examine the multiplication utilities
   //------------------------------------------------------------------------
   printf("\nMultiplication test\n");

   fnum2 = 64.125;
   fnum3 = 0.755;

   qnum2 = floatToFixed(fnum2, 18);
   qnum3 = floatToFixed(fnum3, 18);

   qnum1 = Qn_MULTIPLY(qnum2, qnum3, 18);
   ftemp1 = fixedToFloat(qnum1, 18);

   printf("Product = %g (float) qn product = %d (dec), converted back %g (float)\n", (fnum3 * fnum2), qnum1, ftemp1);

   //------------------------------------------------------------------------
   // Implement floating point scientific equations two different ways
   // using normal fractions and using decimal fractions
   //------------------------------------------------------------------------
   printf("\nComplex calculations test\n");

   //---------------------------------------------------------------------
   // Implement QN scientific equations: x**3-.0001x**2-676X+.0676
   //------------------------------------------------------------------------

   fnum1 = pow(1.0,3) - .0001*(pow(1.0,2)) - 676*1.0 + .0676;
   fnum2 = pow(1.0,3) - (2/10000)*(pow(1.0,2)) - 676*1.0 + (679/2500);

   //x^3
   qnum1 = floatToFixed(1.0, 18);
   qnum4 = qnum1;

   //.0001x^2
   qnum2 = floatToFixed(.0001, 18);
   qnum3 = Qn_MULTIPLY(qnum1, qnum2, 18);

   //x^3 - .0001x^2
   qnum1 -= qnum3;

   //x^3 - .0001x^2 - 676x
   qnum2 = floatToFixed(676.0, 18);
   qnum4 = Qn_MULTIPLY(qnum4, qnum2, 18);
   qnum1 -= qnum4;

   qnum3 = floatToFixed(.0676, 18);
   qnum1 += qnum3;

   fnum3 = fixedToFloat(qnum1, 18);

   // print all 3 answers
   printf("float answer = %g , fractional answer = %g, Qn answer = %g \n", fnum1, fnum2, fnum3);

   //------------------------------------------------------------------------
   // Examine the performance
   //------------------------------------------------------------------------
   printf("\nPerformance test\n");

   fnum1 = 3.1415;
   fnum2 = -674.9325;
   qnum1 = floatToFixed(fnum1,18);
   qnum2 = floatToFixed(fnum2,18);

   // Floating point addition
   time1 = clock();
   for(i = 0; i < LOOP_DELAY; i++) {
      ftemp1 = fnum1 + fnum2;
   }
   time1 = clock()-time1;

   // QN addition
   time2 = clock();
   for(i = 0; i < LOOP_DELAY; i++) {
      qnum3 = qnum1 + qnum2;
   }
   time2 = clock()-time2;

   // Print out the clicks and who was faster
   printf("floating addition clicks is: %ld\n", time1);
   printf("fixed addition clicks is: %ld\n", time2);


   // Floating multiplication
   time1 = clock();
   for(i = 0; i < LOOP_DELAY; i++) {
      fnum1 *= fnum2;
   }
   time1 = clock()-time1;

   // QN multiplication
   time2 = clock();
   for(i = 0; i < LOOP_DELAY; i++) {
      qnum1 *= qnum2;
   }
   time2 = clock()-time2;

   // Print out the clicks and who was faster
   printf("floating multiplication clicks is: %ld\n", time1);
   printf("fixed multiplication clicks is: %ld\n", time2);

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

