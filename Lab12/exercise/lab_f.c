/*---------------------------------------------------------------------------
  This program demonstrates the limitations of scanf() and the use of fseek()
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 256

int main(int argc, char *argv[]) {
   // File name to read and the handle
   FILE *DataFile = NULL;
   char *fileName = "data.txt";

   // The data to parse
   int num, v1, v2, v3;
   char *buffer;

   // Get memory
   buffer = (char *)malloc(BUFF_SIZE * sizeof(char));
   if (NULL == buffer) {
      printf("Malloc error\n");
      exit(99);
   }

   // Open the data
   DataFile = fopen("data.txt", "r");
   if (NULL == DataFile) {
      printf("Error could not open '%s'\n", fileName);
    }

   // Read all the EXACT data in the file using fgets(), in a while loop
   // and print the results out, removing any trailing LF \n character, if any.
   while (fgets(buffer, BUFF_SIZE,DataFile)){
      // remove the lf at the end
      int size = strlen(buffer);
      buffer[size - 1] = '\0';
      printf("Read: '%s'\n", buffer);
   }

   // Use fseek() to start back at the beginning of the file
   fseek(DataFile, 0, SEEK_SET);

   // Read all the data in the file and print the results out
   while (fscanf(DataFile, "%d %d %d", &v1, &v2, &v3) > 0){
	printf("%d parameters %d %d %d\n", 3, v1, v2, v3);
   }

   // Return memory, close files
   fclose(DataFile);
   free(buffer);
   return(0);
}
