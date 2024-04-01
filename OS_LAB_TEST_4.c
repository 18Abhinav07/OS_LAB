#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 100

struct val_threads {
    char *arr_input;
    char *arr_output;
    int ind;
};

void* convert(void *args) {
    struct val_threads *val = (struct val_threads *)args;
    char letter = val->arr_input[val->ind];
    if (islower(letter)) {
        letter = toupper(letter);
    } else if (isupper(letter)) {
        letter = tolower(letter);
    }
    val->arr_output[val->ind] = letter;
    return NULL;
}

int main() {
    char Input[MAX_LENGTH];
    char Output[MAX_LENGTH];

    printf("Enter a sentence: ");
    scanf("%[^\n]", Input);

    int n;
    printf("Enter the number of threads: ");
    scanf("%d", &n);

    pthread_t threads[n];

    int input_length = strlen(Input);
    int j = 0;
    int i = 0;

    while (j < input_length) {
        struct val_threads arg;
        arg.arr_input = Input;
        arg.arr_output = Output;
        arg.ind = j;
        pthread_create(&threads[i], NULL, convert, &arg);
        pthread_join(threads[i], NULL); // Wait for thread to complete
        j++;
        i = (i + 1) % n;
    }

    printf("Converted sentence: %s\n", Output);

    return 0;
}
