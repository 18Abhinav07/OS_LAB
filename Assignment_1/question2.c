//
// Created by abhinav.pangaria22b on 1/15/24.
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    fprintf(stderr,"running count.\n");
    int val;
    int non_alpha = 0;
    do {
        val = getchar();
        if (val == '\n') {
//            non_alpha++;
            continue;
        }
        if ((char)val != EOF) {
            if (!isalpha(val)) {
                non_alpha++;
            }
        } else {
            fprintf(stdout, "The count of non alphabetic character is : %d\n", non_alpha);
            exit(0);
        }
    } while (1);
}
