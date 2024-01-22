//
// Created by abhinav.pangaria22b on 1/15/24.
//
#include <unistd.h>
#include <stdio.h>

int main() {




// // using dup.

//    int fd = dup(STDOUT_FILENO);
//    if (fd == -1) {
//        perror("dup");
//        return 1;
//    }
//    write(fd, "Writing to the standard output via dup.\n", 100);
//    close(fd);

// // using dup2.

//    int fd2 = dup2(STDOUT_FILENO, STDERR_FILENO);
//    if (fd2 == -1) {
//        perror("dup2");
//        return 1;
//    }
//    write(fd2, "Writing to standard error output duplicating from the standard output.\n", 100);
//    close(fd2);


    char buffer[100];
    ssize_t n1 = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (n1 == -1) {
        perror("read error");
        return 1;
    }
    buffer[n1] = '\0';
    printf("Read: %s\n", buffer);


    const char *message = "Writing to the stdout!\n";
    ssize_t n2 = write(STDOUT_FILENO, message, 100);
    if (n2 == -1) {
        perror("write error");
        return 1;
    }

}

