#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phil_num + 4) % N
#define RIGHT (phil_num + 1) % N


int philosopher[N];


class monitor
{
   int state[N];
   pthread_cond_t phil_cond[N];
   pthread_mutex_t mutex;

public:
   
   monitor()
   {
      for(int i = 0; i < N; i++)
      {
        state[i] = THINKING;
      }

      for(int i = 0; i < N; i++)
      {
        pthread_cond_init(&phil_cond[i], NULL);
      }

      pthread_mutex_init(&mutex, NULL);  
   }

   void test(int phil_num)
   {
     if(state[phil_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != 
     EATING)
     {
       state[phil_num] = EATING;
       pthread_cond_signal(&phil_cond[phil_num]);
       // signal that the philosopher is safe to take up the chopsticks.
     } 
   }


    void take_forks(int phil_num)
    {
      // lock the mutex
      pthread_mutex_lock(&mutex);
      
      state[phil_num] = HUNGRY;
      test(phil_num);

      if(state[phil_num] != EATING)
      {
        // wait on the signal for the condtion..
        pthread_cond_wait(&phil_cond[phil_num], &mutex);
      }

      printf("Philosopher %d takes fork %d and %d\n", phil_num + 1, LEFT + 1, 
      phil_num + 1);
      printf("Philosopher %d is Eating\n", phil_num + 1);
      
      // unlock the mutex.
      pthread_mutex_unlock(&mutex);
 
    }

    void return_forks(int phil_num)
    {
      pthread_mutex_lock(&mutex);
      state[phil_num] = THINKING;
      
      printf("Philosopher %d putting fork %d and %d down\n", phil_num + 1, 
      LEFT + 1, phil_num + 1);
      printf("Philosopher %d is thinking\n", phil_num + 1);
      
      test(LEFT);
      test(RIGHT);

      pthread_mutex_unlock(&mutex);
    }

    ~monitor()
    {

        for (int i = 0; i < N; i++) {
            pthread_cond_destroy(&phil_cond[i]);
        }

        pthread_mutex_destroy(&mutex);
    }
};

monitor philosopher_object;

void* philosopher_thread(void* num)
{
  while(1){
    int i = *(int*)num;
    sleep(1);
      philosopher_object.take_forks(i);
    sleep(1);
      philosopher_object.return_forks(i);
  }
}


int main()
{

    // Declaration...
    pthread_t thread_id[N];
    pthread_attr_t attr;

    // Initialization...
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,
                                PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < N; i++) {
        philosopher[i] = i;
    }

    // Creating...
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], &attr, philosopher_thread,
                       &philosopher[i]);
        cout << "Philosopher " << i + 1 << " is thinking..."
             << endl;
    }

    // Joining....
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Destroying
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;
}

