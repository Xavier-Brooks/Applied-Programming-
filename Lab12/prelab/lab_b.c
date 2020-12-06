/*---------------------------------------------------------------------------
  This prints out argv and env variables 
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {
   //your code here
   int j = 0;
   for(int i = 0; i < argc; i++){
	printf("argument %d is: %s\n", i, argv[i]);
   }

   while(env[j] != NULL){
	printf("env[%d] is: %c\n", j, env[j]);
        j++;
   }

   return 0;
}
