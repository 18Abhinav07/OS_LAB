//
// Created by abhinav.pangaria22b on 1/15/24.
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    fprintf(stderr,"Running convert.\n");
    int val;
    do {
        val = getchar();
        if ((char)val == '\n') {
            printf("\n");
            continue;
        }
        if (val != EOF) {
            if (islower(val)) {
                putchar(toupper(val));
            } else if (isupper(val)) {
                putchar(tolower(val));
            } else {
                putchar(val);
            }
        } else {
            printf("\n");
            exit(0);
        }
    } while (1);
}
