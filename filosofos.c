
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

int numFilosofos = 5;
sem_t sem;

void* comer(void *param);

int main(int argc, char *argv[]) {
  pthread_t threads[numFilosofos];
  int par[numFilosofos];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for(int i = 0; i < numFilosofos; i++) {
    par[i] = i+1;
    pthread_create(&threads[i], &attr, comer, &par[i]);
  }
  sem_init(&sem, 0, 4);

  for (int i = 0; i < numFilosofos; i++) {
      pthread_join(threads[i], NULL);
  }

  sem_destroy(&sem);

}
void* comer(void *param) {
  int num= *((int *)param);
  printf("Filosofo %d intentando comer.\n", num);
  sem_wait(&sem);
  printf("Filosofo %d comiendo.\n", num);
  sleep(10);
  printf("Filosofo %d termino de comer.\n", num);
  sem_post(&sem);
  pthread_exit(0);
}
