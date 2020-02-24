#include <stdatomic.h>
#include <stdio.h>
#include <pthread.h>

#define SIZE 10

// A shared array of unsigned integers (non-atomic, no memory model specified)
static unsigned int x[SIZE] = {0,};

// A shared counter (atomic, with memory order sequential consistency)
static atomic_uint counter = 0;

// The same code for all threads.
void *thread(void *parameter)
{
  while(counter < SIZE)
  {
    x[atomic_fetch_add(&counter, 1)] = (unsigned int)pthread_self();
  }
  return NULL;
}

// Create and start two threads and see how their executions interleave.
// Always returns 0.
int main()
{
  // create the two threads
  // create the two threads.
  pthread_t a,b;
  pthread_create(&a, NULL, &thread, NULL);
  pthread_create(&b, NULL, &thread, NULL);

  // wait for both threads to finish
  pthread_join(a, NULL);
  pthread_join(b, NULL);

  // print the contents of the shared array
  for(unsigned int i = 0; i < SIZE; i++)
  {
    printf("x[%u] = %x\n", i, x[i]);
  }

  return 0;
}