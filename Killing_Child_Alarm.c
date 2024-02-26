#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env;

void handle_alarm(int sig) {
    longjmp(env, 1);  // Jump back to setjmp with error code
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

    if (fork() == 0) {  
        for (int i = x; i <= y; i++) {
            if (is_prime(i)) {
                printf("%d\n", i);
            }
        }
        printf("\n");
        exit(0);  // Child terminates
    } else {  // Parent process
        signal(SIGALRM, handle_alarm);  // Set alarm handler
        alarm(z);  // Set alarm for z seconds

        int result = setjmp(env);
        if (result == 0) {
            // Wait for child to finish
            wait(NULL);
            printf("All the prime numbers printed\n");
        } else {
            // Alarm triggered
            kill(0, SIGKILL);  // Kill all child processes
            printf("Calculation is taking too much time\n");
        }
    }

    return 0;
}
