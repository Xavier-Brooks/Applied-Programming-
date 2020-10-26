/*---------------------------------------------------------------------------
  This program demonstrates a simple 2d dynamic array
  10/08/2019   R. Repka    Initial version
  student version

  valgrind --tool=memcheck --leak-check=yes --track-origins=yes
---------------------------------------------------------------------------*/
#include<stdio.h>
#include <stdlib.h>
#include "Timers.h"

void free_matrix_space(double **M, int rows);

/* Dynamically Allocated Arrays */
int main(int argc, char *argv[]) {

   DECLARE_TIMER(mainTimer)
   DECLARE_REPEAT_VAR(repeat_var)
   // row/index counters and row/col sizes
   int i,j;
   int rows = ROWS;
   int cols = COLS;

   // The key trick to a 2D matrix
   double **A = NULL;
//start timing here
   START_TIMER(mainTimer)
   BEGIN_REPEAT_TIMING(REPEAT_NUM, repeat_var)
   // Allocate space for array of pointers
   A = malloc( rows * sizeof(double *));

   // Allocate space for matrix dat elements
   for (i=0; i< rows; i++){
      A[i] = malloc(cols * sizeof(double) );
   }


    #ifndef EN_TIME
    Initialize the matrix to nice data
   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
         A[i][j] = (double)i +(double)j/10.0;
      }
   }

    Print out the matrix
   printf("The %dx%d 2D dynamic matrix\n", rows, cols);
   for (i=0; i< rows; i++){
      for (j=0; j< cols; j++){
         printf("%4.1f\t", A[i][j]);
      }
      printf("\n");
   }

   // The simple return of space

   // Complex return of space
    #endif //!en_time
   free_matrix_space(A, rows);
   END_REPEAT_TIMING
   STOP_TIMER(mainTimer)

   PRINT_TIMER(mainTimer)
   PRINT_RTIMER(mainTimer, REPEAT_NUM)
   return 0;
}

/*---------------------------------------------------------------------------
  This function frees a 2D matrix generated using individual row allocations

  where:  double **M - Point to an array of pointers to free
          int row    - The number of array row elments in M
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
void free_matrix_space(double **M, int rows) {
	int i = 0;
        for(i = 0; i<rows; i++){
		free(M[i]);
	}
	free(M);
	return;
}
