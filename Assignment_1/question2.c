//
// Created by abhinav.pangaria22b on 1/15/24.
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    fprintf(stderr, "running count.\n");
    int val;
    int non_alpha = 0;

    int max_chars = -1;
    if (argc > 2 && argv[1][0], '-') {
        max_chars = atoi(argv[2]);
        printf("max_char %d\n", max_chars);
        printf("%s", argv[0]);
        if (max_chars <= 0) {
            fprintf(stderr, "Error: -n argument must be a positive integer.\n");
            exit(2);
        }
    }
    int total_chars = 0;
    do {
        val = getchar();
        if (val == '\n') {
//            non_alpha++;
            continue;
        }
        if ((char) val != EOF) {
            if (!isalpha(val)) {
                if (total_chars < max_chars) {
                    putchar(val);
                    total_chars++;
                }
                non_alpha++;
            }
        } else {
            fprintf(stdout, "\nThe count of non alphabetic character is : %d\n", non_alpha);
            exit(0);
        }
    } while (1);
}
