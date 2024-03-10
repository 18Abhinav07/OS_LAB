# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

#define N 3
#define M 3
#define P 3


struct arguments {
    int row;
    int col;
    int(*mat1)[M];
    int(*mat2)[P];
    int(*result)[P];
};


void* multiply(void* arg) {
    struct arguments *args = (struct arguments*)arg;
    int row = args->row;
    int col = args->col;
    int sum = 0;

    for (int i = 0; i < M; i++) {
        sum += args->mat1[row][i] * args->mat2[i][col];
    }

    args->result[row][col] = sum;

    pthread_exit(NULL);
}

int main() {
    int mat1[N][M] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int mat2[M][P] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int result[N][P];

    pthread_t threads[N * P];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            struct arguments *args = (struct arguments*)malloc(sizeof(struct arguments));
            args->row = i;
            args->col = j;
            args->mat1 = mat1;
            args->mat2 = mat2;
            args->result = result;

            pthread_create(&threads[i * P + j], NULL, multiply, (void*)args);
        }
    }

    for (int i = 0; i < N * P; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}