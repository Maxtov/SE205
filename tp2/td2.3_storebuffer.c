#include <stdatomic.h>
#include <stdio.h>
#include <pthread.h>

// Two shared variables (neither atomic, nor with a well specified memory order)
static int x = 0;
static int y = 0;

// A shared counter (atomic, with memory order sequential consistency)
static atomic_int counter = 0;

// First thread, accessing shared variables
void *threadA(void *parameter)
{
  x = 1;
  if (y == 0)
    counter++;

  return NULL;
}

// Second thread, accessing shared variables
void *threadB(void *parameter)
{
  y = 1;
  if (x == 0)
    counter++;

  return NULL;
}

// Create and start two threads over and over again and check the outcome.
// Always returns 0.
int main()
{
  unsigned int i = 0;
  while(++i)
  {
    // create the two threads
    pthread_t a,b;
    pthread_create(&a, NULL, &threadA, NULL);
    pthread_create(&b, NULL, &threadB, NULL);

    // wait for both threads to finish
    pthread_join(a, NULL);
    pthread_join(b, NULL);

    // Can this happen? Should this happen?
    if (counter == 2)
    {
      printf("He! (%d)\n", i);
      return 0;
    }

    // reset shared variables before restarting the experiment.
    x = 0;
    y = 0;
    counter = 0;
  }

  return 0;
}