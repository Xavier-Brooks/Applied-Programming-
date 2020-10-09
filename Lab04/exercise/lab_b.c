/****************************************************************************
 The purpose of this example is to exercise, creating, using and destroying
 pass by pointer and pass by value data structures
  student version

 gcc -O1 -Wall -std=c99 -pedantic -g  lab_a.c -o lab_a
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./lab_a
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*---------------------------------------------------------------------------
  The polynomial data structure contains the number of (size) of the
  allocated array of polynomial coefficients
  e.g     nterms = 4;
          polyCoef = malloc(sizeof(double)*4);
---------------------------------------------------------------------------*/
typedef struct {
    unsigned int nterms;      /* number of terms */
    double *polyCoef;         /* coefficients         */
} Poly;

// Function prototypes
void createPoly(Poly *p, int n);
Poly *createPoly_p(int n);
void destroyPoly(Poly *p);
void destroyPoly_p(Poly *p);
void printPolyRef(Poly *p);
void printPolyVal(Poly data);

int main(int argc, char *argv[]) {
   Poly stackPoly;         // Stack data structure
   Poly *poly_p = NULL;           // Pointer

   // Create and use the data structure
   // Print using pass by pointer AND value
   createPoly(&stackPoly, 10);
   printf("Print poly by reference: ");
   printPolyRef(&stackPoly);
   printf("\r\nPrint poly by value: ");
   printPolyVal(stackPoly);
   printf("\r\n");
   destroyPoly(&stackPoly);
   // Create and use the pointer as a data structure
   // Print using pass by pointer AND value
   poly_p = createPoly_p(10);
   printf("Print poly by reference: ");
   printPolyRef(poly_p);
   printf("\r\nPrint poly by value: ");
   printPolyVal(*poly_p);
   printf("\r\n");
   destroyPoly_p(poly_p);
   return 0;
}

/*---------------------------------------------------------------------------
  Create a test polynomial using a pointer to the data structure
  Be sure to initialize the elements aka: polyCoef[i]  2.0*i
---------------------------------------------------------------------------*/
//initializes the structure given a pointer to it
void createPoly(Poly *p, int n){

	(*p).nterms = n;
	(*p).polyCoef = (double *)malloc(n*sizeof(double));
	for(int i = 0;i < n ;i++){
		(*p).polyCoef[i] = 2.0*i;
	}
}

/*---------------------------------------------------------------------------
  Create a test polynomial using a pointer to the data structure
---------------------------------------------------------------------------*/
// Destroy data structure based test polynomial
void destroyPoly(Poly *p){
	(*p).nterms = 0;
	free((*p).polyCoef);
	(*p).polyCoef = NULL; //frees the coefficients pointer in the poly structure
}

//creates a pointer based polynomial and initializes
Poly* createPoly_p(int n){
	Poly *poly_ptr = (Poly *)malloc(sizeof(Poly)); //creates the structure pointer
	poly_ptr->nterms = n;
	poly_ptr->polyCoef = malloc(n*sizeof(double));
	for(int i = 0; i < n; i++){
		(*poly_ptr).polyCoef[i] = (2.0*i);
	}
	return poly_ptr;
}

/*---------------------------------------------------------------------------
  Destroy pointer based test polynomial
---------------------------------------------------------------------------*/
void destroyPoly_p(Poly *p){
	free((*p).polyCoef);
	(*p).polyCoef = NULL;
	free(p);
	p = NULL;
}

/*---------------------------------------------------------------------------
  Prints the polynomial data passing by reference use %3.0f
---------------------------------------------------------------------------*/
void printPolyRef(Poly *p){
	for(int i = 0; i < (*p).nterms; i++){
		printf("%f ", (*p).polyCoef[i]);
	}
}

/*---------------------------------------------------------------------------
  Prints the polynomial data passing by value use %3.0f
---------------------------------------------------------------------------*/
void printPolyVal(Poly data){
   	for(int i = 0; i<data.nterms; i++){
		printf("%f ", data.polyCoef[i]);
	}
}



