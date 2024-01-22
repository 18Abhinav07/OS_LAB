//
// Created by abhinav.pangaria22b on 1/15/24.
//
#include <stdio.h>

int main(int argc, char *argv[]) {
    int i;
    int flag = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            printf("%s ", argv[i]);
            flag = 1;
        }
    }

    if (flag) {
        printf("\n");
    }

    return 0;
}
