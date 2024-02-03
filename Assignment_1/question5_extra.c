#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int pipe1[2], pipe2[2];

    // Creating the needed pipes.
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        // if pipe fails exit the code.
        perror("Pipe creation failed. Exiting the program with status (1).\n");
        return 1;
    }

    // Creating the needed child processes for the parent.
    pid_t child_process1;
    child_process1 = fork();
    if (child_process1 == -1) {
        perror("Forking the first child resulted in an error. Exiting the program.\n");
        return 1;
    } else if (child_process1 == 0) {
        // if successful in making the child process, then this process will just write to the
        // pipe 1 so that it can pass the read characters to the second process.
        // we will have to close the other read and write ends as they can give sigpipe error.
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        char ch;
//        dup2(pipe1[1], STDOUT_FILENO);
        while (1) {
            // Get input character from user

            printf("Enter a character. To end the input regime provide EOF (CTRL + D) in linux systems. (CTRL + Z)"
                   "in windows system.\n");
            if ((ch = getchar()) != EOF) {
                putchar(ch);
            } else {
                putchar(EOF);
                break;
            }



//            // Write the read  character to pipe1 so that the next process can read them from the first pipe.
//            if (write(pipe1[1], &ch, 1) != 1) {
//                perror("Writing to the first pipe resulted in an error. Exiting the code segment. PIPE 1 FAILED.\n");
//                exit(1);
//            }
        }

        close(pipe1[1]);  // Close write end of pipe1
        _exit(0);  // Terminate child process 1
    }

    pid_t child_process2 = fork();
    if (child_process2 == -1) {
        perror("Forking the second child resulted in an error. Exiting the program.\n");
        return 1;
    } else if (child_process2 == 0) {  // Child process 2
        close(pipe1[1]);  // Close unused write end of pipe1 as the process 1 has already done the writing.
        close(pipe2[0]);  // Close unused read end of pipe2 as it is not needed yet.

        char ch;
        while (1) {
            // Read character from pipe1
            if (read(pipe1[0], &ch, 1) != 1) {
                break;  // EOF or error
            }

            // Increment character
            ch++;

            // Write incremented character to pipe2
            if (write(pipe2[1], &ch, 1) != 1) {
                perror("Writing to the second pipe resulted in an error. Exiting the code segment. PIPE 2 FAILED.\n");
                exit(1);
            }
        }

        close(pipe1[0]);  // Close read end of pipe1 as the written character by child 1 in pipe 1 have been read.
        close(pipe2[1]);  // Close write end of pipe2 as the incremented characters are written to pipe2.
        _exit(0);  // Terminate child process 2
    }

    // Parent process
    close(pipe1[0]);  // Close unused read end of pipe1
    close(pipe1[1]);  // Close unused write end of pipe1

    char ch;
    while (1) {
        // Read character from pipe2
        if (read(pipe2[0], &ch, 1) != 1) {
            break;  // EOF or error
        }
        char org_ch = ch;
        // Decrement character
        ch--;

        // Print decremented character
        printf("Received the char '%c' and decrementing the character gives: %c\n", org_ch, ch);
    }

    // Terminate child processes
//    kill(child_process1, SIGTERM);
//    kill(child_process2, SIGTERM);
//    waitpid(child_process1, NULL, 0);
//    waitpid(child_process2, NULL, 0);

    close(child_process1);
    close(child_process2);
    wait(NULL);
    printf("Child processes terminated. Finishing the parent processes.\n");

    close(pipe2[0]);  // Close read end of pipe2

    return 0;
}
