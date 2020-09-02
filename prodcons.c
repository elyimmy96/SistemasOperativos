
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
  pthread_t threadProduce, threadConsume;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  sem_init(&lleno, 0, 6);
  sem_init(&vacio, 0, 6);
  sem_init(&mutex, 0, 1);

  pthread_create(&threadProduce, &attr, produce, NULL);
  pthread_create(&threadConsume, &attr, consume, NULL);

  pthread_join(threadProduce, NULL);
  pthread_join(threadConsume, NULL);

  sem_destroy(&lleno);
  sem_destroy(&vacio);
  sem_destroy(&mutex);

}
void *produce() {
  int cc = 0;
  int i = 0;

  do {
    sem_wait(&vacio);
    sem_wait(&mutex);
    datos[cc] = random() % 100;
    printf("Produje %d\n", datos[cc]);
    cc = (cc < 4) ? cc+1 : 0;
    sem_post(&mutex);
    sem_post(&lleno);
    i++;
  } while(i < 20);
  pthread_exit(0);
}
void *consume() {
  int cc = 0;
  int i = 0;

  while(i < 20) {
    sem_wait(&lleno);
    sem_wait(&mutex);
    printf("Dato anterior %d\n", datos[cc]);
    datos[cc]+=5;
    printf("Dato actual %d\n", datos[cc]);
    cc = (cc < 4) ? cc+1 : 0;
    sem_post(&mutex);
    sem_post(&vacio);
    i++;
    sleep(2);
  }
  pthread_exit(0);
}
