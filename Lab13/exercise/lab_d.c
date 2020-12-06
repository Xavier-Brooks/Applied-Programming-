/*---------------------------------------------------------------------------
  Create a single thread and use pthread_exit()  
  student copy
  
  gcc -g -std=c99 lab14a.c -lpthread -o lab14a
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //For sleep().
#include <pthread.h> //For threads
#include <time.h>

#define THREAD_CNT 3

void *mySimpleThread(void *name);

// Global variables
int taskRC = 0;
volatile int processed = 0;
pthread_mutex_t lock;

int main(int argc, char *argv[]) {
   // Thread data structure
   pthread_t thread_id[THREAD_CNT];

   if(pthread_mutex_init(&lock, NULL) != 0){
      printf("mutex init failed\n");
      return 1;
  }

   int cnt = 0;

   // Pointer to the process return code
   void *rcp;

   // Process index id number
   int num[THREAD_CNT];
   int rc;

  time_t start = time(NULL);

   printf("Main: Starting thread...\n");
   // Student should add the thread create call here

   for(cnt; cnt < THREAD_CNT; cnt++){
        num[cnt] = cnt;
   	rc = pthread_create(&thread_id[cnt], NULL, mySimpleThread, &num[cnt]);

	if(0 == rc){
		printf("Thread %d started\n", cnt);
	}
	else{
		printf("Thread %d failed to start\n", cnt);
	}
   }//end for loop

   printf("Main: All threads started...\n");

   // Student should add wait for the thread to finish
   // when instructed. Always check the join RC as you
   // may get memory leaks otherwise.

   cnt = 0;
   for(cnt; cnt < THREAD_CNT; cnt++){
   	rc = pthread_join(thread_id[cnt], &rcp);
	printf("Thread RC %d, RCP is %d\n", rc, *(int *)rcp);
   }

   // Print status in the main routine
   for (int i = 0; i < 2; i++){
      printf("Main will sleep 1 second. \n");
      sleep(1);
   }

   start = time(NULL) - start;

   printf("time for processing is: %ld\n", start);
   printf("Exiting main thread\n");
   taskRC = 0;
   pthread_exit(&taskRC);
}

/*---------------------------------------------------------------------------
  A thread that prints out it's ID number and sleeps in a loop,
  printing status
---------------------------------------------------------------------------*/
void *mySimpleThread(void *num) {
   for (int i = 0; i < 6; i++){
      printf("     Thread %d, will sleep 1 second. \n", *(int *)num);
      pthread_mutex_lock(&lock);
      processed ++;
      printf("processed is %d\n", processed);
      pthread_mutex_unlock(&lock);
      sleep(1);
   }

  taskRC = *(int *)num;
  return(&taskRC);
}
