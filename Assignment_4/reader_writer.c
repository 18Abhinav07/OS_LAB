# include <pthread.h>
# include <stdio.h>
# include <semaphore.h>
# include <unistd.h>



sem_t writer;
sem_t mutex;
int reader_count = 5;
int writer_count = 5;

int crd = 0;


void *reads(void *args){

  while(1){
    
    sem_wait(&mutex);
    crd++;

    if (crd == 1){
      sem_wait(&writer);
    }

    printf("Currenlty %d readers are reading , most recent reader %d.\n", crd , *(int*)args);
    
    sem_post(&mutex);
    sleep(5);
    
    sem_wait(&mutex);
    crd --;
    printf("Reader %d left.\n",*(int*)args);
    
    if (crd == 0){
      sem_post(&writer);
    } 
    sem_post(&mutex);
    
    sleep(10);
  }  
}

void *writes(void *args){
  while(1){
    sem_wait(&writer);
    printf("Currenlty writing %d.\n", *(int*)args);
    sleep(1);
    printf("Writer %d left.\n", *(int*)args);
    sem_post(&writer);
    sleep(2);
  }
}



int main(){
  pthread_t rid[reader_count];
  pthread_t wid[writer_count];

  int reader_ids[reader_count];
  int writer_ids[writer_count];

  sem_init(&writer, 0, 1);
  sem_init(&mutex, 0, 1);

  for (int i = 0; i < reader_count; i++) {
    reader_ids[i] = i;
    if (pthread_create(&rid[i], NULL, reads, &reader_ids[i]) != 0) {
      perror("pthread_create");
      exit(1);
    }
    sleep(1);
  }

  for (int i = 0; i < writer_count; i++) {
    writer_ids[i] = i;
    if (pthread_create(&wid[i], NULL, writes, &writer_ids[i]) != 0) {
      perror("pthread_create");
      exit(1);
    }
    sleep(1);
  }

  for (int i = 0; i < reader_writer; i++)
    pthread_join(rid[i], NULL);

  for (int i = 0; i < writer_count; i++)
    pthread_join(wid[i], NULL);

  return 0;
}