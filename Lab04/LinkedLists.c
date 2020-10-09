/******************************************************************************
 An student framework implementation of doubly linked lists that holds
 elements containing a 256 character string and a sequence number.
 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
 03/12/2018 - R. Repka     Added pseudo code
 09/17/2019 - R. Repka     fixed minor prototype errors
 09/26/2019 - R. Repka     Added comments to RemoveFromFrontOfLinkedList()
 10/01/2019 - R. Repka     Changed search function to int
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassErrors.h"
#include "LinkedLists.h"


/******************************************************************************
  Use the following pseudo code to help with all the functions in this module

  add 1st node
    LinkedList front = points to node 1,  LinkedList back = points to node 1
      Node 1 next     = NULL  (at the end)
      Node 1 previous = NULL (at the start)

  add 2nd node
    LinkedList front = points to node 1,  LinkedList back = points to node 2
      Node 1 next points to node 2             Node 2 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)    Node 2 previous points to node 1

  add 3rd node
  LinkedList front = points to node 1,  LinkedList back = points to node 3
      Node 1 next points to node 2           Node 2 next points to node 3      Node 3 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)  Node 2 previous points to node 1  Node 3 previous points to node 2

   Note: It is possible to implement with only 2 special cases

   Remove from front of 3 node list
    save the pointer to the data and return it at the end
    LinkedList front = points to node 2,  LinkedList back = points to node 3
      Node 2 next points to node 3           Node 3 next   = NULL  (at the end)
      Node 2 previous = NULL (at the start)  Node 3 previous points to node 2
      free Node but NOT the data
******************************************************************************/

/******************************************************************************
 Initialize the linked list data structure.  Points to nothing, no entries.

 Where: LinkedLists *ListPtr - Pointer to the linked list to create
 Returns: nothing
 Errors: none
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr)
{
	(*ListPtr).NumElements = 0;
	(*ListPtr).FrontPtr = NULL;
	(*ListPtr).BackPtr = NULL;
	return;
} /* InitLinkedList() */


/******************************************************************************
 Adds a node to the back of the list.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to add to
        char *DataPtr - Pointer to the data to add to the list
 Returns: nothing
 Errors: Prints to stderr and exits
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, char *DataPtr)
{
	LinkedListNodes *newNode;
	//allocate the space in memory for the new node
	//and copy the string= data into the node
	newNode = (LinkedListNodes *)malloc(sizeof(LinkedListNodes)); //allocates for new node
	strcpy(newNode->String, DataPtr);
	if((*ListPtr).FrontPtr == NULL){
		(*ListPtr).FrontPtr = newNode;
		(*ListPtr).BackPtr = newNode;
		ListPtr->NumElements += 1;
		newNode->Previous = NULL;
		newNode->Next = NULL;
		return;
	}
	else{
		newNode->Previous = (*ListPtr).BackPtr; //previous of new node points to the old back node
		((ListPtr)->BackPtr)->Next = newNode;
		(*ListPtr).BackPtr = newNode;
		newNode->Next = NULL; //since new node is the latest the next pointer should be null
		(*ListPtr).NumElements += 1;//increment the size since the new node has been added
		return;
	}
} /* AddToBackOfLinkedList */

/******************************************************************************
 Removes a node from the front of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.
 Note: You will need to malloc a string buffer and copy the data from the
       linked list node before destroying the node.  The calling routine was
       written to free the string so there are no memory leaks


   Linked lists contain nodes, which contain data strings

 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the data removed or NULL for none
 Errors: none
******************************************************************************/
char *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr)
{
	LinkedListNodes *NodePtr;
	char *Data;
	Data = (char *)malloc(MAX_BUFF_LEN*sizeof(char));
	NodePtr = (*ListPtr).FrontPtr;
	strcpy(Data, (NodePtr->String)); //copies the data before the front node is removed
	(*ListPtr).FrontPtr = NodePtr->Next; //the structure tracking the front and back of the list is update with a new front node
	free(NodePtr);// removes the node from mem for good
	(*ListPtr).NumElements -= 1; //updates linked list size
	return Data;
} /* RemoveFromFrontOfLinkedList() */


/******************************************************************************
 De-allocates the linked list and resets the struct fields (in the header)
 to indicate that the list is empty.

 If you choose to use the RemoveFromFrontOfLinkedList() function, remember

 the calling routine must free the returned pointer so there are no memory leaks
 Where: LinkedLists *ListPtr    - Pointer to the linked list to destroy
 Returns: nothing
 Errors: none
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr)
{
	char *c;
	while((*ListPtr).NumElements != 0){
		c = RemoveFromFrontOfLinkedList(ListPtr);
		free(c);
	}
	InitLinkedList(ListPtr);
	return;
} /* DestroyLinkedList() */



/******************************************************************************
 Searches the linked list for a provided word. If found, returns the pointer
 to the element struct. If not found, returns a NULL pointer

 Where: LinkedLists *ListPtr - Pointer to the linked list to search
        char *String         - Pointer to the string to search
 Returns: 1 if found, 0 otherwise
 Errors: none
 * ***************************************************************************/
int SearchList(LinkedLists *ListPtr, char *String)
{
/*-----  Don't implement this until instructed in Lab 6 ----*/
return 0;
} /* SearchList() */




