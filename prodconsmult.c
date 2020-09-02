
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t lleno, vacio, mutex;
int datos[4];

void* produce();
void *consume();

int main(int argc, char *argv[]) {
  pthread_t threadProduce[5], threadConsume[5];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  sem_init(&lleno, 0, 0);
  sem_init(&vacio, 0, 5);
  sem_init(&mutex, 0, 1);

  int a[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    pthread_create(&threadProduce[i], &attr, consume, &a[i]);
  }
  for (int i = 0; i < 5; i++) {
    pthread_create(&threadConsume[i], &attr, consume, &a[i]);
  }
  for (int i = 0; i < 5; i++) {
    pthread_join(threadProduce[i], NULL);
  }
  for (int i = 0; i < 5; i++) {
    pthread_join(threadConsume[i], NULL);
  }

  sem_destroy(&lleno);
  sem_destroy(&vacio);
  sem_destroy(&mutex);

}
void *produce(void *param) {
  int cc = 0;
  int i = 0;

  do {
    sem_wait(&vacio);
    sem_wait(&mutex);
    datos[cc] = random() % 100;
    printf("Producer %d: Produje %d\n", *((int *)param), datos[cc]);
    cc = (cc < 4) ? cc+1 : 0;
    sem_post(&mutex);
    sem_post(&lleno);
    i++;
  } while(i < 20);
  pthread_exit(0);
}
void *consume(void *param) {
  int cc = 0;
  int i = 0;

  while(i < 20) {
    sem_wait(&lleno);
    sem_wait(&mutex);
    printf("Consumer %d: Dato anterior %d\n", *((int *)param), datos[cc]);
    datos[cc]+=5;
    printf("Consumer %d: Dato actual %d\n", *((int *)param), datos[cc]);
    cc = (cc < 4) ? cc+1 : 0;
    sem_post(&mutex);
    sem_post(&vacio);
    i++;
    sleep(2);
  }
  pthread_exit(0);
}
