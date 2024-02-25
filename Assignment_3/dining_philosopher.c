#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (ph_num + 4) % N
#define RIGHT (ph_num + 1) % N

sem_t mutex;
sem_t S[N];
int state[N];

void test(int ph_num) {

  if (state[ph_num] == HUNGRY && state[LEFT] != EATING &&
      state[RIGHT] != EATING) {
    state[ph_num] = EATING;
    printf("Philosopher %d takes fork %d and %d\n", ph_num + 1, LEFT + 1,
           ph_num + 1);
    printf("Philosopher %d is Eating\n", ph_num + 1);
    sleep(1);
    sem_post(&S[ph_num]);
  }
}

void take_fork(int ph_num) {
  sem_wait(&mutex);
  state[ph_num] = HUNGRY;
  printf("Philosopher %d is Hungry\n", ph_num + 1);
  test(ph_num);
  sem_post(&mutex);
  sem_wait(&S[ph_num]);
}

void put_fork(int ph_num) {
  sem_wait(&mutex);
  state[ph_num] = THINKING;
  printf("Philosopher %d putting fork %d and %d down\n", ph_num + 1, LEFT + 1,
         ph_num + 1);
  printf("Philosopher %d is thinking\n", ph_num + 1);
  test(LEFT);
  test(RIGHT);
  sem_post(&mutex);
}

void *philosopher(void *num) {
  while (1) {
    int *i = num;
    take_fork(*i);
    put_fork(*i);
  }
}

int main() {
  int i;
  pthread_t thread_id[N];
  sem_init(&mutex, 0, 1);
  for (i = 0; i < N; i++)
    sem_init(&S[i], 0, 0);

  int phil_ids[N];
  for (i = 0; i < N; i++) {
    phil_ids[i] = i;
    pthread_create(&thread_id[i], NULL, philosopher, &phil_ids[i]);
    printf("Philosopher %d is thinking\n", i + 1);
    sleep(5);
  }

  for (i = 0; i < N; i++)
    pthread_join(thread_id[i], NULL);

  return 0;
}
