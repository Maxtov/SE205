#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static const unsigned int SIZE = 2000;

typedef struct my_list_entry_t
{
  unsigned int value;
  struct my_list_entry_t *next;
} my_list_entry_t;

// A shared list of thread IDs.
my_list_entry_t *x = NULL;

// An atomic counter variable used to track the number of list insertions
// executed.
atomic_uint insertion_counter = 0;

// insert a new value at the head of the linked list.
my_list_entry_t *my_list_insert(my_list_entry_t *head, unsigned int value)
{
  // allocate new list entry
  my_list_entry_t *new_entry;
  new_entry = (my_list_entry_t *)malloc(sizeof(my_list_entry_t));

  // initialize new list entry
  new_entry->next = head;
  new_entry->value = value;

  // track number of insertions
  atomic_fetch_add(&insertion_counter, 1);

  // return new entry as new first element of the list
  return new_entry;
}

// count the number of elements in the linked list.
unsigned int my_list_size(my_list_entry_t *head)
{
  unsigned int counter = 0;
  while(head != NULL)
  {
    counter++;
    head = head->next;
  }

  return counter;
}

// The same code for all threads.
void *thread(void *parameter)
{
  unsigned int done = 0;
  while(!done)
  {
    if (my_list_size(x) < SIZE)
    {
      x = my_list_insert(x, (unsigned int)pthread_self());
    }
    else
    {
      done = 1;
    }
  }

  // should never be reached
  return NULL;
}

// Create and start two threads and observe the interleavings, synchronizing 
// accesses to a shared list using a home-brew mutex implementation.
// Always returns 0.
int main()
{
  // create the two threads
  pthread_t a,b;
  pthread_create(&a, NULL, &thread, NULL);
  pthread_create(&b, NULL, &thread, NULL);

  // wait for both threads to finish
  pthread_join(a, NULL);
  pthread_join(b, NULL);

  // print the contents of the shared lis
  unsigned int i = 0;
  while(x)
  {
    printf("x[%u] = %x\n", i++, x->value);
    x=x->next;
  }

  printf("insertions: %u (%u)\n", insertion_counter, SIZE);

  return 0;
}