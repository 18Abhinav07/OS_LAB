# include <pthread.h>
# include <stdio.h>
# include <semaphore.h>
# include <unistd.h>

int max_buf = 10;
int counter = 0;
sem_t mutex;

void *producer(void *args){
  do {
  
  while(counter == max_buf){
    printf("Buffer is full\n");
    sem_post(&mutex);
    sleep(10);
  }
    
  sem_wait(&mutex);
  counter = counter + 1;
  printf("Produced %d\n", counter);
  sem_post(&mutex);
  sleep(3);
  }while(1);
}

void *consumer(void *args){
  do{
  
    
  while(counter == 0){
    printf("Buffer is empty\n"); 
    sem_post(&mutex);
    sleep(10);
  }
    
  sem_wait(&mutex);
  counter = counter - 1;
  printf("Consumed %d\n", counter);
  sem_post(&mutex);
  sleep(1);
  }while(1);
  
}

int main(){

  pthread_t pid;
  pthread_t cid;
  sem_init(&mutex, 0, 1);

  pthread_create(&cid, NULL, consumer, NULL);
  pthread_create(&pid, NULL, producer, NULL);

  pthread_join(pid, NULL);
  pthread_join(cid, NULL);

  sem_destroy(&mutex);
  return 0;
}
