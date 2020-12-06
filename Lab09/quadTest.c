/**************************************************************************
  quadTest.c - for the student
  This program demonstrates C99's complex support.

 12/22/2018 - R. Repka  Initial version
 03/27/2019 - R. Repka  b**2 typo
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include </usr/include/complex.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/************************************************************************
   Simple code to test complex support
************************************************************************/
int main(int argc, char* argv[]){

/*--------------------------------------------
timing prep initialization
----------------------------------------------*/
DECLARE_TIMER(rootTimer);
DECLARE_REPEAT_VAR(rootRep);

/*---------------------------------------
UI variables with sentimental value
----------------------------------------*/
double num1 = NAN; //a
double num2 = NAN; //b
double num3 = NAN; //c

/*-----------------------------------------
variables here control getopt_long_only cmd
line parsing utility
-----------------------------------------*/
int rc;
//getopt
int option_index = 0;

//contains the short command line paraneters list
char *getoptOptions = "a:b:c:";

//contains the long command line parameter list, mainly matches short list
struct option long_options[] = {
{"numa", required_argument, 0, 'a'},
{"numb", required_argument, 0, 'b'},
{"numc", required_argument, 0, 'c'},
{0, 0, 0, 0}
};

//enables automatic error reporting
opterr = 1;
while((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1){

//optional string debugging
//printf("getopt_long_only returned = '%c' index '%d'\n", rc, option_index);
switch(rc){
	case 'a':
	   num1 = atof(optarg);
	   break;

	case 'b':
	   num2 = atof(optarg);
	   break;

	case 'c':
	   num3 = atof(optarg);
	   break;

	case '?':
	   break;

	default:
	   printf("Internal error: undefined option %0xX\n",rc);
	   exit(PGM_INTERNAL_ERROR);

   }//End Switches
} /*end while*/

/*--------------------------------------------------------------
checking command line syntax errors
--------------------------------------------------------------*/
if((optind < argc)){
	fprintf(stderr, "Testing real/complex root finding\n");
	fprintf(stderr, "usage: -num[a] | -num[b] | -num[c]\n");
	fprintf(stderr, "e.g:  ./quadTest -numa 1 -numb 0 -numc 1 \n");
	fflush(stderr);
	return(PGM_INTERNAL_ERROR);
}/*end if error*/

printf("num1 is %f, num2 is %f, num3 is %f\n", num1,  num2, num3);

if(isnan(num1) || isnan(num2) || isnan(num3)){
	printf("Missing one or more arguments try again\n\n");
	return(0);
}

/*1
roots - pointer to final root calculations in memory
cpoly - polynomial data structure being declared
*p - pointer to the polynomial being worked on
*/
double complex *roots = malloc(2*sizeof(double complex));
polynomial cpoly;
polynomial *p = &cpoly;

//initializes the users polynomial
initPoly(p, 3);
p->polyCoef[0] = num1 + 0.0*I;
p->polyCoef[1] = num2 + 0.0*I;
p->polyCoef[2] = num3 + 0.0*I;
printf("The polynomial is fully initialized begin root...\n");

START_TIMER(rootTimer)
BEGIN_REPEAT_TIMING(REPEAT_NUM,rootRep)
quadraticRoots(p, roots);
END_REPEAT_TIMING
STOP_TIMER(rootTimer)

PRINT_TIMER(rootTimer)
PRINT_RTIMER(rootTimer, REPEAT_NUM)

printf("root of: %.2fx^2 + %.2fx + %.2f is:\n", num1, num2, num3);
printf("root 1: %.2f + %.2fI\n", creal(roots[0]), cimag(roots[0]));
printf("root 2: %.2f + %.2fI\n", creal(roots[1]), cimag(roots[1]));

freePoly(p);
free(roots);
return 0;

} /* End main */


 /*---------------------------------------------------------------------------
  Returns the two roots from a quadratic polynomial

    =  -b +-sqrt(b**2-4ac)
       ----------------------
                 2a

  Where: polynomial *p              - Pointer to a 2nd order polynomial
  Returns: double complex* rootList - A pointer to a complex root pair.
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void quadraticRoots(polynomial *p, double complex *rootList){

	double a = creal((*p).polyCoef[0]);

	//if a is 0 the no solution exist thus no point in finding b, c, and discriminant
	if(a == 0.0){
		printf("There is no solution a is 0\n\n");
		freePoly(p);
		free(rootList);
		exit(0);
	}

	//a not 0 let's continue with calculation
	else{
		double b = creal((*p).polyCoef[1]);
		double c = creal((*p).polyCoef[2]);
		double discriminant = (b*b) - (4*a*c);

		//completely real roots to be found no imaginary parts
		if(discriminant > 0.0){
			double rsqrt = sqrt(discriminant); //real positive root of quadratic formula
			rootList[0] = (-b + rsqrt)/(2.0*a) + 0.0*I; //real root1
			rootList[1] = (-b - rsqrt)/(2.0*a) + 0.0*I; //real root 2
		}

		//same real root
		else if(discriminant == 0.0){
			rootList[0] = -b/(2.0*a) + 0.0*I;
			rootList[1] = rootList[0];
		}

		//two unreal roots to be found
		else{
			double complex croot = 0.0 + csqrt(discriminant); //imaginary root from quadratic formula
			rootList[0] = (-b/(2.0*a)) + (cimag(croot)/(2.0*a))*I; //quadratic formula performed for complex root1
			rootList[1] = (-b/(2.0*a)) - (cimag(croot)/(2.0*a))*I; //quadratic formula performed for complex root2
		}
	}
}


/*---------------------------------------------------------------------------
  This function allocates an array of "nterm" double complex elements and then
  initializes the terms of the polynomial to NAN (not a number).
  We initialize the terms of the polynomial to NAN to prevent the users from
  accidentally using the polynomial before valid values are set.

  Where: polynomial *p       - Pointer to polynomial data structure to create
         unsigned int nterms - The number of elements to create
  Returns: nothing
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void initPoly(polynomial *p, unsigned int nterms){

	(*p).nterms = nterms;
	(*p).polyCoef = malloc(nterms * sizeof(double complex));
	if(NULL == (*p).polyCoef){
		printf("failed to allocate coefficient array\n");
		exit(MALLOC_ERROR);
	}

	for(int i = 0; i<nterms; i++){
		(*p).polyCoef[i] = NAN;
	}
}


/*---------------------------------------------------------------------------
  Destroys/frees a polynomial

  Where: polynomial *p - Pointer to polynomial data structure to destroy
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void freePoly(polynomial *p){
	free((*p).polyCoef);
	p = NULL;

}


