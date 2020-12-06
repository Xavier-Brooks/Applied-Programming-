/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random doubles
 *
 */

#include <stdio.h>
#include <stdlib.h> /* for qsort()    */

/* Function prototypes */
int cmpdbl(const void *p1,const void *p2); /* for qsort() */

/*
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it
*/

//returns -2 if number of cmd paramters is <=1
//returns -3 if number of requested elements is <2

int main(int argc, char *argv[]) {
   double *array;
   int num;
   // Check the command line entry
   if(argc != 2){
	printf("Terminating program: Only one parameter required\n");
        return -2;
   }

   num = atoi(argv[1]);

   if(num < 2){
	printf("Terminating program: Requested elements amount is 2 minimum\n");
        return -3;
   }

   // Get the memory
   array = (double *) malloc(num * sizeof(double));

   // Initialize the array with random data
   for(int i = 0; i < num; ++i) {
      array[i] = (rand() % 1001) /10.0 - 50.0;
   }

   // Sort the data
   qsort(array, num, sizeof(double), cmpdbl);

   // Print the sorted dat
   for(int i = 0; i < num; ++i) {
      printf("% 10.1f\n", array[i]);
   }

   free(array);

   printf("\n");
return(0);
}

/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to doubles in this case
---------------------------------------------------------------------------*/
int cmpdbl(const void *p1, const void *p2) {

        if( *(double *)p1 == *(double *)p2 ){
		return 0;
	}
	else if(*(double *)p1 < *(double *)p2){
		return -1;
	}

	return 1;
}


