/******************************************************************************
* FILE: hello_mutex.c
* DESCRIPTION:
*   This program demonstrates the use of mutexes to avoid data inconsistency
*   in a scenario where multiple threads attempt to access shared resources.
*   It specifically tests for mutex functionality and potential deadlocks
*   by having two tasks, taskA and taskB, which lock and unlock two mutexes
*   in different orders.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lockdep.h>

#define NUM_THREADS	5
#define NUM_TEST	5

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// Task A function: locks mutex1 then mutex2, simulating a resource access pattern
void *taskA(void *threadid)
{
   long tid;
   int i;
   tid = (long)threadid;
   printf("task A! It's me, thread #%ld!\n", tid);

   for(i = 0; i < NUM_TEST; i++) {
	   mutex_lock(&mutex1);
	   sleep(1);
	   mutex_lock(&mutex2);
	   printf("task A in loopi %d \n",i );
	   sleep(1);
	   mutex_unlock(&mutex2);
	   sleep(1);
	   mutex_unlock(&mutex1);
	   sleep(1);
   }
   pthread_exit(NULL);
}

// Task B function: locks mutex2 then mutex1, simulating a different resource access pattern
void *taskB(void *threadid)
{
   long tid;
   int i;
   tid = (long)threadid;
   printf("task B! It's me, thread #%ld!\n", tid);

   for(i = 0; i < NUM_TEST; i++) {
	   mutex_lock(&mutex2);
	   sleep(1);
	   mutex_lock(&mutex1);
	   printf("task B in loopi %d \n",i );
	   sleep(1);
	   mutex_unlock(&mutex1);
	   sleep(1);
	   mutex_unlock(&mutex2);
	   sleep(1);
   }
   pthread_exit(NULL);
}

// Main function: creates threads to run taskA and taskB
int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   t = 0;
   rc = pthread_create(&threads[t], NULL, taskA, (void *)t);
   t++;
   rc = pthread_create(&threads[t], NULL, taskB, (void *)t);
   if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
