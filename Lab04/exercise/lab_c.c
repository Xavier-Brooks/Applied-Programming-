 /****************************************************************************
 The purpose of this example is to exercise, using pointers
 
 student version 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_c.c -o lab_c
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./lab_c
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------
  These data structures are used to simulated a very simple linked list
---------------------------------------------------------------------------*/
typedef struct Node{
    int index;
    struct Node *next;
    char string[40];
} Node;

typedef struct {
    int nterms;
    Node *front;
} Wrapper;

// Function prototypes
void printList(Wrapper *wrapper_p);

int main(int argc, char *argv[]) {
   Wrapper *wrapper_p;
   Node *node_p;
   Node *temp_p;

   //creates space in memry for data and structures
   node_p = (Node *)malloc(sizeof(Node));
   temp_p = (Node *)malloc(sizeof(Node));
   wrapper_p = (Wrapper *)malloc(sizeof(Wrapper));

   // Wire the first node in
   strcpy((*temp_p).string, "one");
   (*temp_p).index = 1;
   (*temp_p).next = node_p;

   (*wrapper_p).nterms = 1;
   (*wrapper_p).front = temp_p; //front points to the first node

   // Wire the second node in
   strcpy((*node_p).string, "two");
   (*node_p).index = 2;
   (*wrapper_p).nterms = 2;

   //print the linked list nodes
   printList(wrapper_p);

   // Clean up memory
   free(temp_p);
   free(node_p);
   free(wrapper_p);

   return 0;
}

// Walk the linked list and print the contents
void printList(Wrapper *wrapper_p){

     Node *pointer_p; //points to each node in linked list
     pointer_p = (*wrapper_p).front; //initializes pointer_p to point to the first node of our LL
     int i = 1; //starting at node 1
     printf("Printing %d node linked list \r\n", (*wrapper_p).nterms); //wrapper holds the numer of LL nodes
     while(i < ((*wrapper_p).nterms + 1)){ //while (i<#ofnodes+1)
	printf("node %d contains the string %s \r\n", (*pointer_p).index, (*pointer_p).string);
        pointer_p = (*pointer_p).next;
	i++;//iterates loop until I is greater than the number of nodes
     }
     pointer_p = NULL;
     return; //everything is printed return to main
}
