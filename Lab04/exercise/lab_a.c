	/****************************************************************************
 The purpose of this example is to exercise using data and function pointers.
 It also demonstrates the value of using NULL assignments to identify pointer
 issues.
 
 student version 
 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_b.c -o lab_b
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./lab_b
 
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
   int entry;
   char string [40];
} Data;

int fun2x(int num);
int fun3x(int num);

Data ramData;


int main(int argc, char *argv[]) {
   int(*function_p)(int);
   int answer;
   Data stackData;
   static Data staticData;
   Data *heapData_p;
   
   heapData_p = (Data *)malloc(sizeof(Data));
 
   // Print out the addresses of items
   printf("Program %8p \r\n", &main);
   printf("Static data %8p \r\n", &staticData);
   printf("Ram data %8p \r\n", &ramData);
   printf("Heap Data %8p \r\n", &heapData_p);
   printf("Stack data %8p \r\n", &stackData);

   // Using function pointers
   function_p = &fun2x;
   answer = (*function_p)(2);
   printf("Passing the 2x function a 2, i got %i \r\n", answer);

   function_p = &fun3x;
   answer = (*function_p)(2);
   printf("Passing the 3x function a 2, i got %i \r\n", answer);

    // Initialize the stack data
    stackData.entry = 1;
    strcpy(stackData.string, "Stack");
    printf("stack data values before free: %i '%s' \r\n", stackData.entry, stackData.string);

    //Initialize the heap data, use it or free it
    heapData_p->entry = 2;
    strcpy(heapData_p->string,"Pointer");
    printf("Heap data values before free: %i '%s' \r\n", heapData_p->entry, heapData_p->string);

    printf("Heap data values after free: %i '%s' \r\n", heapData_p->entry, heapData_p->string);

    return 0;
}


int fun2x(int num) {
   return(num*2);
}
int fun3x(int num) {
   return(num*3);
}
