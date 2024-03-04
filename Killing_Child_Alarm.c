#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/wait.h>

jmp_buf env;

void handle_alarm(int sig) {
    longjmp(env, 1);  
}

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main() {
    int x, y, z;

    printf("Enter three numbers: ");
    scanf("%d %d %d", &x, &y, &z);
    if(z == 0){
      printf("Calculations cannot be performed as there is no time to execute the given query.\n");
      exit(0);
    }

    if (fork() == 0) {  
        for (int i = x; i <= y; i++) {
            if (is_prime(i)) {
                printf("%d\n", i);
            }
        }
        printf("\n");
        exit(0);  
    } else {  
        signal(SIGALRM, handle_alarm);  
        alarm(z); 

        int result = setjmp(env);
        if (result == 0) {
           
            wait(NULL);
            printf("All the prime numbers printed\n");
        } else {
            printf("Calculation is taking too much time\n");
            kill(0, SIGKILL);   
        }
    }

    return 0;
}
