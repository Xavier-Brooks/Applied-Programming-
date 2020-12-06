/*-----------------------------------------------------------------------
  Simple program to dump IEEE single precision floating point numbers
  student copy
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIGN_MASK	  0x80000000
#define SIGN_SHIFT        31
#define EXPONENT_MASK     0x7F800000
#define EXPONENT_SHIFT    23
#define EXPONENT_BIAS     0x7F
#define SIGNIFICAND_MASK  0x007FFFFF

void ieeePrint(float fnum);

union ifloat{
	float fnum;
	int i;
};

int main(int argc, char * argv[]) {

      // Must have a number
   if (2 == argc) {
      ieeePrint(atof(argv[1]));
   }
   else {
      printf("This decodes IEEE single precision numbers\n");
      printf("Error: Syntax is: %s number\n", argv[0]);
   }

   return(0);
}


void ieeePrint(float fnum) {
   int sign, exponent, significand;

   // We require a union because C won't let us mask floating point
   union ifloat ieee;

   //break the floating point item into the proper parts
   ieee.fnum = fnum;
   sign = (ieee.i & SIGN_MASK); //returns the sign bit as is and zeroes the other fields
   sign = (sign >> SIGN_SHIFT);    //returns the sign as either integer 0 or integer 1.

   exponent = (ieee.i & EXPONENT_MASK); //returns the float with all fields except the exponent bits zeroed
   exponent = (exponent >> EXPONENT_SHIFT); //returns the float exponent as an integer (includes bias).
   exponent -= EXPONENT_BIAS; //removes the bias from the exponent

   significand = (ieee.i & SIGNIFICAND_MASK);
   significand = (significand << 1);

   printf("Number %f => Bin sign:%d Dec exponent:%d  Hex significand .%x\n", ieee.fnum, sign, exponent, significand);
   return;
}

