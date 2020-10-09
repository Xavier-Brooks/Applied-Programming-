/****************************************************************************
 Lab 3 student file
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   char *text_p;
   int age = 22;
   int height = 6;
   printf("I am %d years old.\n", age);
   printf("I am %d inches tall.\n\n", height);

   // allocate memory for our text string and check for errors
   // use calloc to make valgrind happy
   text_p = calloc( 12, sizeof(char) );
   if(NULL == text_p) {
      fprintf(stderr, "Error - unable to allocate memory!\n");
   }

   // Put data into the text buffer
   strncpy( text_p, "This string is way to long and will corrupt memory", 12);
   printf("The string contains '%s'\n", text_p);

   free(text_p);
   text_p = NULL;       // always do this
   return 0;
}
