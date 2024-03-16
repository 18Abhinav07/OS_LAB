#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct value_threads {
    int *arr;
    int begin;
    int end;
};

void *sort_half_array(void *args) {
    struct value_threads *val = (struct value_threads *)args;

    for (int i = val->begin; i < val->end; i++) {
        int min_val = val->arr[i];
        int min_ind = i;
        for (int j = i + 1; j < val->end; j++) {
            if (val->arr[j] < min_val) {
                min_val = val->arr[j];
                min_ind = j;
            }
        }
        int a = val->arr[i];
        val->arr[i] = val->arr[min_ind];
        val->arr[min_ind] = a;
    }
    pthread_exit(NULL);
}

void merge_split_arrays(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

int main() {
    int size = 0;
    printf("Enter the number of elements that the array will store:\n");
    scanf("%d", &size);

    int array[size];

    printf("Please enter the values that will be present in the array:\n");
    for (int i = 0; i < size; i++) {
        printf("Enter the %d value: ", i + 1);
        scanf("%d", &array[i]);
    }

    struct value_threads args_1, args_2;
    args_1.arr = array;
    args_1.begin = 0;
    args_1.end = size / 2;

    args_2.arr = array;
    args_2.begin = size / 2;
    args_2.end = size;

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, sort_half_array, &args_1);
    pthread_create(&thread2, NULL, sort_half_array, &args_2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    merge_split_arrays(array, args_1.begin, (args_1.end - 1), (args_2.end - 1));

    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
