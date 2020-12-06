/*********************************************************************
* Example: Using the GSL (Gnu Scientific Library) to solve a system
* of algebraic equations Ax=b via permuted LU factorization.
*
* student version
*
* Compile: gcc -o lu lu_fact.c -lgsl -lgslcblas
*
*
* 12/03/2016   R. Repka - Fixed ansi warning for DECLARE_TIMER()
* 11/29/2019   R. Repka - Added true dynamic support
* Reference: Golub and Vanloan Algorithms 3.4.1
*********************************************************************/
#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "ClassErrors.h"

#ifndef INITIAL_READ_SIZE
   #define INITIAL_READ_SIZE  (20)
#endif
#ifndef READ_GROWTH_SIZE
  #define READ_GROWTH_SIZE   (20)
#endif

// Used to return the size of GSL matrix rows and columns
#define GSL_MAT_ROWS(x)    (x->size1)
#define GSL_MAT_COLS(x)    (x->size2)

//file io defines
#define BUFF_SIZE 256

// Function prototypes
gsl_matrix* initMatrix(FILE *file);
int readMatrix(FILE *file, gsl_vector *V, gsl_matrix *M);

/*---------------------------------------------------------------------------

---------------------------------------------------------------------------*/
int main (int argc, char* argv[]) {
    int  i, j, errorCode = 0;
    FILE *inHandle;

    // Allocate the GSL variables
    gsl_matrix *mat;
    gsl_matrix *mat_c;
    gsl_vector *b, *x;
    gsl_permutation *p;
    int s;

   /*------------------------------------------------------------------------
     These variables are used to control the getopt_long_only command line
     parsing utility.
   ------------------------------------------------------------------------*/
   int rc;
   char *inFile = NULL;
   int verb     = 0;
   int showData = 0;

   // getopt_long stores the option index here.
   int option_index = 0;

   // This contains the short command line parameters list
   char *getoptOptions = "i:vd";

   /* This contains the long command line parameter list, it should mostly
     match the short list.  Note: get_opt_long is not really needed       */
   struct option long_options[] = {
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"data",    no_argument, 0, 'd'},
      {"in",      required_argument, 0, 'i'},
      {"input",   required_argument, 0, 'i'},
      {"verb",    no_argument, 0, 'v'},
      {"verbose", no_argument, 0, 'v'},
      {0, 0, 0, 0}
   };

   opterr = 1;           // Enable automatic error reporting
   while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options,
                                                    &option_index)) != -1) {
   //  printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);
      /* Detect the end of the options. */
      switch (rc)
        {
        case 'i':
          inFile = optarg;
          break;

        case 'v':
          verb = 1;
           break;

        case 'd':
          showData = 1;
           break;

        case '?':  // Handled by the default error handler
          break;

       default:
          printf ("Internal error: undefined option %0xX\n", rc);
          exit(PGM_INTERNAL_ERROR);
       } // End switch
   } // end while

   /*------------------------------------------------------------------------
     Check for command line syntax errors like not all parameters specified
     or the first parameter is zero.
   ------------------------------------------------------------------------*/
   if ((optind < argc) || (NULL == inFile)) {
      fprintf(stderr, "This program using GSL PLU factorization to solve a system \n");
      fprintf(stderr, "of algebraic equations via Ax=b\n");
      fprintf(stderr, "usage: hw12  -i[n[put]] file [-v[erb[ose] [-d[ata]\n");
      fprintf(stderr, "Where: -input file - the matrix file to process.\n");
      fprintf(stderr, "                     first line contains the number\n");
      fprintf(stderr, "                     rows and columns in the subsequent\n");
      fprintf(stderr, "                     data\n");
      fprintf(stderr,"        -data       - Print the input A and b data\n");
      fprintf(stderr, "       -verbose    - Enable optional debugging information\n\n");
      fprintf(stderr, "e.g.   hw12 -i rand.txt \n");
      return(PGM_SYNTAX_ERROR);
   } /* End if error */

   // Try to open the matrix
   inHandle = fopen(inFile, "r");
   if (NULL == inHandle){
      fprintf(stdout, "Error: input file '%s' not found\n", inFile);
      return(FILE_NOT_FOUND);
   }

   // The file is there, initialize the matrix from the file
   mat = initMatrix(inHandle);

   // We must have a good matrix file, allocate the data arrays
   b = gsl_vector_calloc(GSL_MAT_ROWS(mat));
   x = gsl_vector_calloc(GSL_MAT_ROWS(mat));
   p = gsl_permutation_alloc(GSL_MAT_ROWS(mat));

   // Read the data into the GSL matrix.
   errorCode = readMatrix(inHandle, b, mat);
   if (errorCode) {
      // There is something wrong with the matrix file
      return(errorCode);
   } /* End else */

   if (showData) {
      printf("Matrix A:\n");
      for (i = 0; i < GSL_MAT_ROWS(mat); i++) {
          for (j = 0; j < GSL_MAT_COLS(mat); j++)
              printf ("%8.4f ", gsl_matrix_get(mat, i, j));
      putchar('\n');
      }

   // Print entries of vector b
   printf("Vector b:\n");
   gsl_vector_fprintf (stdout, b,"%8.4f") ;
   }

   //copy of mat without the vector column
   mat_c = gsl_matrix_alloc( mat->size1, (mat->size2 - 1));
   for(int i = 0; i < GSL_MAT_ROWS(mat); i++){
	for(int j = 0; j < GSL_MAT_COLS(mat) - 1; j++){
		gsl_matrix_set(mat_c, i, j, gsl_matrix_get(mat, i, j));
	}
   }

   // Do the LU decomposition
   errorCode = gsl_linalg_LU_decomp (mat_c, p, &s);

   if (verb) {
      fprintf(stdout, "\nP = [");
      gsl_permutation_fprintf (stdout, p, " %u");
      fprintf(stdout, " ] \n");

      fprintf(stdout, "LU matrix = \n");
      printf("Matrix LU:\n");
      for (i = 0; i < GSL_MAT_ROWS(mat); i++) {
         for (j = 0; j < GSL_MAT_COLS(mat); j++) {
            printf ("%7.2g ", gsl_matrix_get (mat, i, j));
         }
      putchar('\n');
      } // End for i
   } // End if verbose

   // Was the LU good?
   if(0 != errorCode){
      fprintf(stdout, "Error: Matrix does not have a solution. \n");
      return(NO_SOLUTION);
   } /* End if */

   // We have a good LU, solve it
   gsl_linalg_LU_solve (mat_c, p, b, x);

   printf("Solution x:\n");
   gsl_vector_fprintf (stdout, x, "%8.4f");

   // Free things
   gsl_matrix_free(mat);
   gsl_matrix_free(mat_c);
   gsl_vector_free(b);
   gsl_permutation_free(p);
   gsl_vector_free(x);

   fclose(inHandle);
   return(PGM_SUCCESS);
} /* main */


/*---------------------------------------------------------------------------
  This function allocates a GSL Matrix array from the first row of data supplied.
  The first line will have two numbers (separated by spaces), specifying the
  dimensions of the augmented matrix G = [A b].

  This routine supports input data of arbitrary length.

  The following lines will have the rows of G, each entry separated by spaces.
  The example below shows the two first lines of the data file for a system of
  5 equations with 5 unknowns

  # indicates a comment, must be at the top of the file
  5            6
  1.1       3.4       5.6       7.8       7.8       1.1

  Where:   FILE *file - Open file handle to a matrix data text file
  Returns: gsl_matrix * - an empty GSL matrix file of the correct size

  Error policy: Prints an error message and exit with a unique error code
---------------------------------------------------------------------------*/
gsl_matrix* initMatrix(FILE *file){
   char *buffer;
   char *token;
   int found_size_line = 0, nr = 0, nc = 0;
   int num_elem = 0;

   buffer = (char *)malloc(BUFF_SIZE*sizeof(char));

   //iterates through file lines until first non commented line is found
   while(fgets(buffer, BUFF_SIZE, file)){
        if (buffer[0] == '#'){
		continue;
	}
	else{
	  found_size_line = 1;
          break;
	}
   }//end while

   //if the file is completely commented out
   if(found_size_line == 0){
	printf("all lines were commented\n");
	free(buffer);
        fclose(file);
	exit(DATA_READ_ERROR);
   }

   token = strtok(buffer, " \r\t\n"); //first number in the line holding matrix size
   while(token != NULL){ //if null then there was no row or column in size line
	if(num_elem == 0){ //first integer encountered is number of rows
		nr = atoi(token);
	}
        else if(num_elem == 1){ //second integer encountered is number of columns
		nc = atoi(token);
	}
	token = strtok(NULL, " \r\t\n");
	num_elem ++; //increment num_elem if more than row and column size are on the line
   }

   if(nc != (nr + 1) ){
	printf("Column value incorrect\n");
	free(buffer);
	fclose(file);
	exit(DATA_READ_ERROR);
   }

   if(num_elem <= 1){
	printf("Missing either row or column size\n");
	free(buffer);
	fclose(file);
	exit(DATA_READ_ERROR);
   }

   if((num_elem > 2)){
	printf("Too many columns of data\n");
	free(buffer);
	fclose(file);
	exit(DATA_READ_ERROR);
   }

   if((nr < 2)){
	printf("must have at least 2 rows\n");
	free(buffer);
	fclose(file);
	exit(DATA_READ_ERROR);
   }

   gsl_matrix *M;
   M = gsl_matrix_calloc(nr, nc);
   free(buffer);
   return M;
} // initMatrix


/*---------------------------------------------------------------------------
  This function populates a GSL matrix from a supplied text file of the form


  The first line will have two numbers (separated by spaces), specifying the
  dimensions of the augmented matrix G = [A b].

  The following lines will have the rows of G, each entry separated by spaces.
  The example below shows the two first lines of the data file for a system of
  5 equations with 5 unknowns

  # indicates a comment, must be at the top of the file
  5            6
  1.1       3.4       5.6       7.8       7.8       1.1

  This routine supports input data of arbitrary length

  Where: FILE *file - Open file handle to a matrix text file, with the first line
                      already processed
         gsl_vector *V  - Pointer to a vector to hold the "b" vector
         gsl_matrix *M  - Pointer to a matrix to hold the "A" matrix
  Errors: none
  Returns: int readMatrix - 0 = success, otherwise
                            4 = too many columns
                            5 = too many rows
---------------------------------------------------------------------------*/
int readMatrix(FILE *file, gsl_vector *V, gsl_matrix *M){

   char *buffer;
   buffer = (char *)malloc(BUFF_SIZE * sizeof(char));

   char *token;
   int r = M->size1, r_check = 0;
   int c = M->size2, c_check = 0;

   //iterates through the matrix in the txt file line by line
   while(fgets(buffer, BUFF_SIZE, file)){

	r_check++; //increment number of rows before parsing it for matrix

	/*
	* r is number of rows(not row index)
	* if r_check is the row number being populated
	* if r_check > r than number of rows specified in the text file was wrong
	*/
	if(r_check > r){
		printf("To many Rows\n");
		free(buffer);
		gsl_vector_free(V);
		gsl_matrix_free(M);
                fclose(file);
		return 5;
	}

	token = strtok(buffer, " \r\n\t");
        while(token != NULL){

		if(c_check == c){
			printf("Too much columns of data\n");
			free(buffer);
			gsl_vector_free(V);
			gsl_matrix_free(M);
                        fclose(file);
			return 4;
		}

		double x = (double) atof(token);

		//if the last column is reached, using c-1 for indexing
		if( c_check == (c-1) ){
			gsl_vector_set(V, r_check - 1, x);
		}

		gsl_matrix_set(M, r_check - 1, c_check, x);

		c_check++;
		token = strtok(NULL, " \r\n\t");
	}

        /*
	*if c_check < c then there are less columns than specified in one of the rows
	*/
	if(c_check < c - 1 ){
		printf("Not enough columns of data\n");
		free(buffer);
		gsl_vector_free(V);
		gsl_matrix_free(M);
                fclose(file);
		return 4;
	}

	c_check = 0; //resets the column location to zero to populate next row

   }//end while

   /*
   * the while loop above terminates when no rows are left
   * if r_check < r than there were less rows than specified in the text file
   */
   if(r_check < r){
	printf("Not enough Rows\n");
	free(buffer);
	gsl_vector_free(V);
	gsl_matrix_free(M);
        fclose(file);
	return 5;
   }

   free(buffer);
   return 0;
} // end readMatrix
