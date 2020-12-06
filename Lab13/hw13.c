//  This fills ram with +3 sequential integers
//  student file
//
//   gcc -g -O0 -std=c99 hw13.c -lpthread -o hw13 -Wall -pedantic
//  valgrind --tool=memcheck --leak-check=yes ./hw13 -f -s

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#define EN_TIME
#include "Timers.h"
#include "ClassErrors.h"

/*--------------------------------------------------------------------------
  Local data structures and defines
--------------------------------------------------------------------------*/
// Use the larger data size of the Linux cluster, smaller for
// a typical PC.
// guarantees divisible by 2/3/4/5/7/8  Note: this number is VERY large
// and MAY cause overflow issues so order of operations will be
// important.  aka    a*b/c  needs to be written as a*(b/c)  because
// it results in a smaller intermediate value
#define DATA_SIZE           (136*3*5*7*146*512)
#define VALGRIND_DATA_SIZE  (30*3*5*7*8*1024)

// Maximum number of threads
#define MAX_THREADS     (8)

// The number of iterations to slow down thread execution
#define DELAY_LOOPS_EXP        (5)

// The percentage rate to update thread progress
#define STATUS_UPDATE_RATE (10)

// Thread information control structure
  struct ThreadData_s {
     int threadID;      // Contains the thread ID number 0..n
     int segSize;       // The amount of total work for this thread
                        // threadID*segSize is the starting index of
                        // the data to initialize
     int *dataPtr;      // Pointer to the one contagious array buffer
                        // that each thread will work on
     int trackStatus;   // Flag to identify if status updates should be reported
     int verbose;       // Flag to indicate if the task should run in verbose mode
  };

/* Function prototypes */
void *do_process(void *data);

/* Used to control access to the progress counter */
int Processed = 0;
pthread_mutex_t lock;
int threadarray[MAX_THREADS];

int main(int argc, char *argv[]) {
   /*------------------------------------------------------------------------
     General purpose variables
   ------------------------------------------------------------------------*/
   DECLARE_TIMER(timer);
   DECLARE_REPEAT_VAR(x)
   time_t  wallTime;    // Used to report wall execution time.

   /*------------------------------------------------------------------------
     Thread process information
   ------------------------------------------------------------------------*/
   struct ThreadData_s *thread_s;
   pthread_t *threads;
   int *inData;

   /*------------------------------------------------------------------------
      UI variables with sentential values
   ------------------------------------------------------------------------*/
   int numThreads = 0;
   int verbose = 0;
   int sizebit = 0, status_flag = 0;
   int dataSize = 0;
   void *rcp = NULL;

   /*------------------------------------------------------------------------
   variables here control getopt_long_only cmd line parsing utility
   ------------------------------------------------------------------------*/
   int rc = 0;
   int option_index = 0;
   char *getoptOptions = "t:s::v::f::";

   /*------------------------------------------------------------------------
   contains the long command lnie paremeter list, mainly
   matches the short list
   -------------------------------------------------------------------------*/
   struct option long_option[] = {
   {"threads", required_argument, 0, 't'},
   {"status", optional_argument, 0, 's'},
   {"verbose", optional_argument, 0, 'v'},
   {"fast", optional_argument, 0, 'f'},
   {0,0,0,0}
   };

   opterr = 1;
   while((rc = getopt_long_only(argc, argv, getoptOptions, long_option, &option_index)) != -1){
   	switch(rc){
		case 't':
			numThreads = atoi(optarg);
			break;

		case 's':
			status_flag = 1;
			break;

		case 'v':
			verbose = 1;
			break;

		case 'f':
			sizebit = 1;
			break;

		default:
			printf("Internal error: undefined option %0xX\n", rc);
			exit(PGM_INTERNAL_ERROR);
	}
   }

   /*------------------------------------------------------------------------
     Check for command line syntax errors
   ------------------------------------------------------------------------*/
   if ((optind < argc) || (numThreads < 1) || (numThreads > 8) ){
      fprintf(stderr, "This program demonstrates threading performance.\n");
      fprintf(stderr, "usage: hw13 -t[hreads] num [-s[tatus]] [-f[ast]] [-v[erbose]] \n");
      fprintf(stderr, "Where: -t[hreads] num - number of threads 1 to %d,required\n", MAX_THREADS);
      fprintf(stderr, "       -s[tatus]      - display thread progress, optional\n");
      fprintf(stderr, "       -v[erbose]     - verbose flag, optional\n");
      fprintf(stderr, "       -f[ast]        - shorter run for Valgrind, optional\n");
      fprintf(stderr, "eg: hw7 -t 3 -status\n");
      fflush(stderr);
      return(PGM_SYNTAX_ERROR);
   } /* End if error */

   //adjust the data size being used according to cmd line parameters
   if(sizebit == 0){
	dataSize = DATA_SIZE;
   }
   else{
	dataSize = VALGRIND_DATA_SIZE;
   }

   START_TIMER(timer)
   BEGIN_REPEAT_TIMING(1,x)
   wallTime = time(NULL);
   //initializes 1D array, thread objects, and thread Data structures dynamically
   thread_s = (struct ThreadData_s *)malloc(numThreads * sizeof(struct ThreadData_s));
   threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
   inData = (int *)malloc(dataSize * sizeof(int));
   int chunk_size =  dataSize/numThreads;

   //allocation check
   if((inData == NULL) || (threads == NULL) || (thread_s == NULL)){
	printf("Failed at memory allocation exiting program\n");
        free(thread_s);
	free(threads);
	free(inData);
	exit(MALLOC_ERROR);
   }

   // Print message before starting the timer
   printf("\nStarting %d threads generating %d numbers\n\n", numThreads, dataSize);

   //initializes the mutex for the progress print statements
   if(pthread_mutex_init(&lock, NULL) != 0){
	printf("mutex initialization failed\n");
	free(thread_s);
	free(threads);
	free(inData);
	exit(PGM_INTERNAL_ERROR);
   }

   // Spin up N threads
   for(int i = 0; i < numThreads; i++) {
      // Build the thread specific information
      thread_s[i].threadID = i;
      thread_s[i].segSize = chunk_size;
      thread_s[i].dataPtr = &inData[i*chunk_size];
      thread_s[i].trackStatus = status_flag;
      thread_s[i].verbose = verbose;

      // Start the thread
      pthread_create(&threads[i],  NULL,  do_process, &(thread_s[i]));

      if (verbose) {
         fprintf(stdout, "Thread:%d  ID:%ld started\n", i, (unsigned long int)threads[i]);
      } // End threads
}

   /* Print out the progress status */
   while((Processed < dataSize) && status_flag){
	pthread_mutex_lock(&lock);
	fprintf(stdout, "Processed: %d line %f percent complete\n", Processed, 100.0*((float)Processed/(float)dataSize));
        fflush(stdout);
	pthread_mutex_unlock(&lock);
	sleep(1);
   }

   /* Wait for all processes to end */
   for(int i = 0; i < numThreads; i++) {
	int rc = pthread_join(threads[i], &rcp);
	printf("Task %d join %d, task %d\n", i, rc, *(int *) rcp);
   } // End threads
   END_REPEAT_TIMING
   STOP_TIMER(timer)
   PRINT_TIMER(timer)
   wallTime = time(NULL) - wallTime;
   fprintf(stderr, "Total wall time = %d sec\n", (int)wallTime);

   printf("Verifying results...  ");
   for (int i = 0; i < dataSize; i++) {
       if (inData [i] != 3*i) {
         printf("Error inData[%d]= %d != %d\n", i, inData[i], 3*i);
	 free(inData);
	 free(threads);
	 free(thread_s);
         exit(PGM_INTERNAL_ERROR);}
   } // End verification
   printf("success\n\n");

   // Clean up
   free(thread_s);
   free(threads);
   free(inData);
   pthread_exit(NULL);

   } // End main


/****************************************************************************
  This threading process will initialize parts of a very large array by 3's
  It contains code to SLOW execution down so that status updates can be easily
  seen.  The function prototype is defined by pthread so we MUST use it.

  void *do_process(void *data)
  Where: void *data - pointer to some user defined data structure
                      We will use struct ThreadData_s
  Returns: void *   - pointer to some user defined return code structure
                      We will use an integer array
  Errors: none

****************************************************************************/
void *do_process(void *data) {

   struct ThreadData_s *tdata = data;

   
   int segSize = tdata->segSize;
   int track_status = tdata->trackStatus;
   int counter = 0;
   int limit = STATUS_UPDATE_RATE*(segSize/100);

   // Print out the thread status
   if (tdata->verbose) {
      fprintf(stdout, "Thread:%d  track status:%d  seg size:%dKB  data ptr:%p\n",  tdata->threadID, tdata->trackStatus, segSize, (void *)(tdata->dataPtr));
      fflush(stdout);
   } // End verbose

   int start = (tdata->threadID) * segSize;
   // Process all the data in the portion of the array
   for (int i = start; i < start+segSize; i++) {
      tdata->dataPtr[i-start] = 3*i;

      // Slow the CPU
      int delay = 1<<DELAY_LOOPS_EXP;
      while (delay){
         delay--;
      }//end while

      counter++;
      //track status
      if(track_status && (counter >= limit)){
        pthread_mutex_lock(&lock);
	Processed += counter;
	pthread_mutex_unlock(&lock);
	counter = 0;

      // Print out the thread status
      if (tdata->verbose){
         fprintf(stdout, "%d", tdata->threadID);
         fflush(stdout);
      } // End verbose if
    }
   } // End for loop

   // There might be some status left to update
   if(track_status){
	pthread_mutex_lock(&lock);
	Processed+=counter;
	pthread_mutex_unlock(&lock);
   }

   threadarray[tdata->threadID] = 10 + tdata->threadID;

   // Return the task ID number + 10
   pthread_exit(&threadarray[tdata->threadID]);
} // End do_process
