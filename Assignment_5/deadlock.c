#include <stdio.h>
#include <stdbool.h>

#define MAX_RESOURCES 3
#define MAX_PROCESSES 5

void detect_deadlock(int num_resources, int resource_instances[], int allocation_matrix[MAX_PROCESSES][MAX_RESOURCES], int request_matrix[MAX_PROCESSES][MAX_RESOURCES]) {
    bool deadlock = false;
    int checked_count = 0;
    bool checked[MAX_PROCESSES] = {false};
    int deadlock_processes[MAX_PROCESSES];
    int deadlock_count = 0;

    while (checked_count != MAX_PROCESSES) {
        int min_res_pro = -1;
        int dead = -1;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            bool sat = true;
            if (checked[i]) {
                continue;
            }
            int j;
            for (j = 0; j < MAX_RESOURCES; j++) {
                if (request_matrix[i][j] > resource_instances[j]) {
                    sat = false;
                    break;
                }
            }

            if (sat) {
                min_res_pro = i;
            } else {
                dead = i;
                continue;
            }
        }  

        if (min_res_pro != -1) {
            checked[min_res_pro] = true;
            checked_count++;
            for (int j = 0; j < MAX_RESOURCES; j++) {
                resource_instances[j] += allocation_matrix[min_res_pro][j];
            }
        } else {
            if (checked_count != MAX_PROCESSES) {
                deadlock = true;
                deadlock_processes[deadlock_count++] = dead;
                printf("Terminating the process %d\n", dead);
                checked[dead] = true;
                checked_count++;
                for (int j = 0; j < MAX_RESOURCES; j++) {
                    resource_instances[j] += allocation_matrix[dead][j];
                }
            } else {
                printf("All process checked.\n");
            }
        }
    }

    if (deadlock) {
        printf("Deadlock was detected.\n");
        for (int j = 0; j < deadlock_count; j++) {
            printf("Process %d was terminated for deadlock removal.\n", deadlock_processes[j]);
        }
    } else {
        printf("No deadlock detected.\n");
    }
}

int main() {
    int num_resources = 3;
    int resource_instances[] = {3, 5, 2};
    int allocation_matrix[MAX_PROCESSES][MAX_RESOURCES] = {
        {0, 1, 0},
        {1, 0, 2},
        {1, 3, 1},
        {0, 0, 1},
        {4, 2, 3}
    };
    int request_matrix[MAX_PROCESSES][MAX_RESOURCES] = {
        {1, 1, 0},
        {1, 2, 3},
        {3, 1, 1},
        {0, 0, 1},
        {3, 2, 2}
    };

    detect_deadlock(num_resources, resource_instances, allocation_matrix, request_matrix);

    return 0;
}
