#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int pipe1[2], pipe2[2];

    pipe(pipe1);
    pipe(pipe2);

    int child1 = fork();

    if (child1 == 0) {
        // This is the first child process
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        printf("Enter a character. EOF to exit\n");
        do {
            int ch;
            ch = getchar();
            if (ch == EOF) {
                ch = EOF;
                write(pipe1[1], &ch, sizeof(ch));
                close(pipe1[1]);
                break;
            }
            if (ch == 10) {
                sleep(1);
                continue;
            }
            write(pipe1[1], &ch, sizeof(ch));
            sleep(1);
        } while (1);
    } else {
        int child2 = fork();

        if (child2 == 0) {
            // This is the second child process
            close(pipe1[1]);
            close(pipe2[0]);

            do {
                int ch;
                read(pipe1[0], &ch, sizeof(ch));
                if (ch != EOF) {
                    ch++;
                }
                if (ch == EOF) {
                    ch = EOF;
                    write(pipe1[1], &ch, sizeof(ch));
                    close(pipe1[0]);
                    close(pipe2[1]);
                    break;
                }
                printf("Printing %d from the intermediate process with id. %d. :: ", ch, getpid());
                putchar(ch);
                printf("\n");
                write(pipe2[1], &ch, sizeof(ch));
                sleep(1);
            } while (1);
        } else {
            // This is the parent process
            close(pipe1[1]);
            close(pipe1[0]);
            close(pipe2[1]);
            int status;

            do {
                int ch = EOF;
                read(pipe2[0], &ch, sizeof(ch));
                if (ch != EOF) {
                    ch--;
                }
                if (ch == EOF || (waitpid(-1, &status, WNOHANG) != 0)) {
                    ch = EOF;
                    close(pipe2[0]);
                    printf("Received an end of file . Terminating all the processes. EXITING.\n");
                    break;
                }
                printf("Printing %d from the parent process with id. %d. :: ", ch, getpid());
                putchar(ch);
                fflush(stdout);
                printf("\n");
                sleep(1);
            } while (1);
        }
    }
    wait(NULL);
    wait(NULL);

    return 0;
}
