#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

int is_prime(int num) {
    if(num <= 1) return 0;
    for(int i = 2; i*i <= num; i++) {
        if(num % i == 0) return 0;
    }
    return 1;
}

void handler(int signum) {
    if (signum == SIGALRM) {
        printf("Calculation is taking too much time\n");
        exit(0);
    }
}

int main() {
    int x;
    int y;
    int z;
    printf("Please enter the respective values of x,y and z.\n");
    scanf("%d%d%d", &x, &y, &z);

    if(z == 0){
      printf("Calculations cannot be performed as there is no time to execute the given query.\n");
      exit(0);
    }

    jmp_buf environment_buffer;
    signal(SIGALRM, handler);

    if (setjmp(environment_buffer) == 0) {
        int pid = fork();

        if (pid == 0) {
            alarm(z);
            for (int i = x; i <= y; i++) {
                if (is_prime(i)) {
                    printf("%d is prime.\n", i);
                }
            }
        } else if (pid > 0) {
            wait(NULL);
            printf("All the prime numbers from %d to %d that can be printed within %d seconds are printed\n",x,y,z);
            longjmp(environment_buffer, 1);
        }
    }

    return 0;
}