//
// Created by abhinav.pangaria22b on 1/22/24.
//
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdbool.h>
#include "fcntl.h"
#include "stdio.h"
#include "setjmp.h"

#define BUFFER_SIZE 1024

jmp_buf env;

void killChildren(int pid) {
    kill(pid, SIGINT);
}

void handler() {
    fprintf(stderr, "Handler is running\n");
    longjmp(env, 1);
}


int main(int argc, char *argv[]) {

    int overlay = 0;
    int readFile = 0;
    int writeFile = 0;

    int overlayIndex = -1;
    int readFileIndex = -1;
    int writeFileIndex = -1;

    argc -= 1; // This is to remove the filename of the executable passed

    if (argc == 3) {
        //Only one condition
        overlay = 1;
        readFile = 1;
        writeFile = 1;

        overlayIndex = 1;
        readFileIndex = 2;
        writeFileIndex = 3;

    } else if (argc == 2) {
        //Two Conditions
        if (argv[1][0] != '-') {
            readFile = 1;
            readFileIndex = 1;

            writeFile = 1;
            writeFileIndex = 2;
        } else {
            overlay = 1;
            overlayIndex = 1;

            readFile = 1;
            readFileIndex = 2;
        }

    } else if (argc == 1) {
        //Two conditions

        if (argv[1][0] != '-') {
            readFile = 1;
            readFileIndex = 1;
        } else {
            overlay = 1;
            overlayIndex = 1;
        }
    } else {
        // No conditions
        //Nothing to write
    }

    printf("Overlay Status: %d || Overlay Index: %d\n", overlay, overlayIndex);

    printf("ReadFile Status: %d ||ReadFile Index: %d \n", readFile, readFileIndex);

    printf("WriteFile Status: %d || WriteFile Index: %d\n", writeFile, writeFileIndex);

    int readfd = STDIN_FILENO;
    if (readFile) {
        readfd = open(argv[readFileIndex], O_RDONLY, 0644);
    }

    int writefd = STDOUT_FILENO;

    if (writeFile) {
        writefd = creat(argv[writeFileIndex], 0644);
    }

    if (readfd != -1 && writefd != -1) {


        int oldstdin = dup(0);
        int oldstdout = dup(1);
        int p[2];
        if (pipe(p) < 0) {
            fprintf(stderr, "Pipe had some errors\n");
            exit(1);
        }

        signal(SIGALRM, handler);
        int pid1, pid2;
        //pid 1 is for count
        //pid 2 is for convert

        alarm(1);

        if ((pid1 = fork()) != 0) {
            //This is for the parent

            if ((pid2 = fork()) != 0) {
                //This is for the parent
                close(p[1]);// This is to close the write end to indicate the EOF
                if (setjmp(env) != 0) {
                    // This means that the setjmp is not working properly
                    //printf("SetJMP")
                    killChildren(pid1);
                    killChildren(pid2);
                    printf("Process ID of children are %d and %d\n", pid1, pid2);
                }
                int status, pid;
                while ((pid = wait(&status)) > 0) {
                    //This means that there is a child to wait for
                    printf("Status is %d\n", status);
                    fprintf(stderr, "child pid=%d reaped with exit status=%d\n",
                            pid, WEXITSTATUS(status));
                    if (status != 0 && pid == pid1) {
                        killChildren(pid1);
                        killChildren(pid2);
                    }
                }


            } else {
                // This is for the convert
                dup2(p[1], STDOUT_FILENO); // Convert should write to pipe
                dup2(readfd, STDIN_FILENO); // Convert should read from file
//                sleep(200); # make this sleep if you want to show kill.
                execl("convert.o", (char *) NULL);
                perror("Error when running convert.o ");
                exit(1);
            }


        } else {
            // This is for the count
            //The program that reads from pipe must be called first


            close(p[1]);// This is to close the write end to indicate the EOF

            dup2(p[0], STDIN_FILENO); //Count should read from  pipe 0
            dup2(writefd, STDOUT_FILENO); //Count should write to write file

            if (overlay == 1) {
                printf("Overlay is there");
                execl("count.o", argv[overlayIndex], (char *) NULL);

            } else {
                execl("count.o", (char *) NULL);
            }

            perror("Error when running count.o");
            exit(1);


        }


    } else {
        fprintf(stderr, "File could not be opened\n");
        exit(0);
    }

    alarm(0);
    return 0;
}