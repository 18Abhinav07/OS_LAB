//
// Created by abhinav.pangaria22b on 1/15/24.
//


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf buffer;

void kill_alarm(int pid) {
    fprintf(stderr, "Killing child, abnormal exit..\n");
    kill(getpid(), SIGKILL);
    exit(0);
}

int main(int argc, char **argv) {

    if (argc == 2) {
        int fd_in = open(argv[1], 0644);
        if (fd_in == -1) {
            perror("Cannot read file1");
        }
        dup2(fd_in, STDIN_FILENO);

    } else if (argc == 3) {
        int fd_in = open(argv[1], 0644);
        if (fd_in == -1) {
            perror("Cannot read file1");
        }

        int fd_out = creat(argv[2], 0644);
        if (fd_out == -1) {
            perror("Cannot creat file2");
        }

//        // Redirecting the file descriptors.
//
//        close(STDIN_FILENO);
//        if (dup(fd_in) == -1) {
//            perror("error in dup");
//            return 1;
//        }
//
//
//        if (dup2(fd_out, STDOUT_FILENO) == -1) {
//            perror("error in dup2");
//            return 1;
//        }
//
//
//        char buffer[1000];
//        ssize_t n1 = read(STDIN_FILENO, buffer, 100 - 1);
//        if (n1 == -1) {
//            perror("read error");
//            return 1;
//        }
//        buffer[n1] = '\0';
//        printf("Read text from file 1 displayed in file 2.: %s\n", buffer);


        // Pipe and forking.

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            return 1;
        }

        pid_t child1 = fork();

        setjmp(buffer);

        if (child1 != 0) {
            pid_t child2 = fork();
            signal(SIGALRM, kill_alarm);
            alarm(1);

            if (child2 != 0) {
                close(pipefd[1]);
//                sleep(10);
                wait(NULL);
                wait(NULL);
                exit(0);
            } else {
                dup2(fd_in, STDIN_FILENO);
                dup2(pipefd[1], STDOUT_FILENO);
                if (execl("./convert.out", "convert", (char *) NULL) == -1) {
                    fprintf(stderr, "error in execl.\n");
                    return 1;
                }
                fprintf(stderr, "successfully run convert.");
            }

        } else {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            dup2(fd_out, STDOUT_FILENO);

            if (execl("./count.out", "count", (char *) NULL) ==
                -1) {
                fprintf(stderr, "error in execl.\n");
                return 1;
            }
            fprintf(stderr, "successfully run count.");

        }
        longjmp(buffer, 1);
    }
    fprintf(stderr, "Normal Exit.\n");
}