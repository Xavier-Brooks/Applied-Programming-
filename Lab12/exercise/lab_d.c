/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random polar data
 *
 * Use compiler directive -DN=size to change the size of the array
 *
 * Reference: A. Kelley and I Pohl "A book on C" 4th Ed.
 * Revised: 3/31/2015 (JCCK)
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h> /* for qsort()    */

/* The polar data structure */
typedef struct {double mag; double ang;} polar;

/* Add your polar compare function prototype here */
int magnitude_compare(const void *p1, const void *p2);
int angle_compare(const void *p1, const void *p2);

/*
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it
*/
int main(int argc, char *argv[]) {

   polar *array;
   int num;
   int i;

   // Check the command line entry
   if(argc != 2){
	printf("Try Again! only one cmd argument necessary\n");
        return -1;
   }

  //checks if the user requested the minimum elements
   num = atoi(argv[1]);
   if(num < 2){
	printf("Try Again! 2 elements is the minimum\n");
        return -1;
   }

   // Get the memory
   array = (polar *) malloc(num * sizeof(polar));
   if(array == NULL){
	printf("Failed on allocation\n");
	return -1;
   }

   // Initialize the array with random data, make one magnitude identical
   for(i = 0; i < num-1; ++i) {
      array[i].mag = (rand() % 1001) /10.0 - 50.0;
      array[i].ang = (rand() % 1001) /10.0 - 50.0;
   }

   // Identical
   array[i].mag = array[i-1].mag;
   array[i].ang = (rand() % 1001) /10.0 - 50.0;

   // Sort the data
      //magnitude sorted first
   qsort(array, num, sizeof(polar), magnitude_compare);

   // Print the sorted dat
   for(i = 0; i < num; ++i) {
      printf("%10.1f %10.1f\n", array[i].mag, array[i].ang);
   }
   printf("\n");

   free(array);

return(0);
}

/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to polar in this case
---------------------------------------------------------------------------*/
// add your function and code here
int magnitude_compare(const void *p1, const void *p2){
	if( fabs((*(polar *)p1).mag - (*(polar *)p2).mag) < FLT_EPSILON){
		if( (*(polar *)p1).ang < (*(polar *)p2).ang) {
			return -1;
		}

		else if( (*(polar *)p1).ang  > (*(polar *)p2).ang ){
			return 1;
		}

		else{
			return 0;

        	}
	}

	else if( (*(polar *)p1).mag < (*(polar *)p2).mag){
		return -1;
	}

	return 1;
}
