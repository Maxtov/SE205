#include <stdio.h>
#include <pthread.h>

void * mythread(void * parameter){
  printf("Hello_ Thread %lu!\n", pthread_self());
  return NULL;
}

int main(){
  pthread_t th1;
  pthread_t th2;
  pthread_create(&th1,NULL,mythread,NULL);
  pthread_create(&th2,NULL,mythread,NULL);

  pthread_join(th1,NULL);//wait end of the thread
  pthread_join(th2,NULL);
  printf("Hello Main !\n");
  return 0;
}
