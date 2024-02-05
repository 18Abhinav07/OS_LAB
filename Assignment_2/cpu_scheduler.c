//
// Created by abhinav.pangaria22b on 2/5/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    char *process_name;
    int arrival_time;
    int end_time;
    int cpu_burst_time;
    int turn_around_time;
    int wait_time;
} Process;

#define MAX_PROCESS 10
int TOTAL_PROCESS = 0;
int current_time = 0;


Process *processTable[MAX_PROCESS];

void readProcessTable(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open this file.\n");
        exit(1);
    }


    while (1) {
        Process *data = (Process *) malloc(sizeof(Process) * 1);
        data->process_name = (char *) malloc(sizeof(char) * 10);
        if (fscanf(file, "%s %d %d\n", data->process_name, &data->arrival_time, &data->cpu_burst_time) == EOF) {
            break;
        }
        processTable[TOTAL_PROCESS] = data;
        TOTAL_PROCESS += 1;
    }

    fclose(file);
    return;
}

void printProcessTable() {
    int i = 0;
    for (i; i < TOTAL_PROCESS; i++) {
        printf("process_name: %s, arrival_time: %d, cpu_burst_time: %d\n", processTable[i]->process_name,
               processTable[i]->arrival_time,
               processTable[i]->cpu_burst_time);
    }
}

void printStatistics() {
    int i = 0;
    float avg_turn_around_time = 0.00;
    float avg_wait_time = 0.00;
    printf("%-20s", "Turnaround Times:");
    for (i; i < TOTAL_PROCESS; i++) {
        printf("%s[%d], ", processTable[i]->process_name, processTable[i]->turn_around_time);
        avg_turn_around_time += processTable[i]->turn_around_time;
    }
    printf("\n");

    i = 0;
    printf("%-20s", "Wait Times:");
    for (i; i < TOTAL_PROCESS; i++) {
        printf("%s[%d], ", processTable[i]->process_name, processTable[i]->wait_time);
        avg_wait_time += processTable[i]->wait_time;
    }
    printf("\n");

    printf("Average turnaround time: %.2f\nAverage wait time: %.2f \n", (avg_turn_around_time / (TOTAL_PROCESS)),
           (avg_wait_time / (TOTAL_PROCESS)));
}

void FCFS() {
    current_time = 0;
    int i = 0;
    printf("----------------------------------------------------\n");
    printf("         FIRST COME FIRST SERVE SCHEDULING.\n");
    printf("----------------------------------------------------\n");
    for (i; i < TOTAL_PROCESS; i++) {
        int cpu_time = processTable[i]->cpu_burst_time;
        char *name = processTable[i]->process_name;

        processTable[i]->wait_time = current_time - processTable[i]->arrival_time;
        processTable[i]->end_time = current_time + cpu_time;
        processTable[i]->turn_around_time = processTable[i]->end_time - processTable[i]->arrival_time;

        printf("[%2d - %-2d] %5s running.\n", current_time, processTable[i]->end_time, name);
        current_time += cpu_time;
    }
    printStatistics();
    return;
}


void RR() {

    int quantum = 0;
    printf("Please enter a time quantum.\n");
    scanf("%d", &quantum);

    current_time = 0;
    int cpuBurst[TOTAL_PROCESS];

    int i = 0;
    for (; i < TOTAL_PROCESS; i++) {
        cpuBurst[i] = processTable[i]->cpu_burst_time;
    }
    printf("----------------------------------------------------\n");
    printf("               ROUND ROBIN SCHEDULING..\n");
    printf("----------------------------------------------------\n");

    while (1) {
        int i = 0;
        for (; i < TOTAL_PROCESS; i++) {
            if (current_time < processTable[i]->arrival_time) {
                break;
            }
            if (cpuBurst[i] == 0) {
                continue;
            }

            char *name = processTable[i]->process_name;
            printf("[%2d - %-2d] %5s running.\n", current_time,
                   ((cpuBurst[i] > quantum) ? (current_time + quantum) : (current_time + cpuBurst[i])), name);

            if (cpuBurst[i] > quantum) {
                cpuBurst[i] -= quantum;
                current_time += quantum;
            } else {
                current_time += cpuBurst[i];
                cpuBurst[i] = 0;

                processTable[i]->end_time = current_time;
                processTable[i]->turn_around_time = processTable[i]->end_time - processTable[i]->arrival_time;
                processTable[i]->wait_time = processTable[i]->turn_around_time - processTable[i]->cpu_burst_time;
            }
        }
        int j = 0;
        int comp_pro = 0;
        for (; j < TOTAL_PROCESS; j++) {
            if (cpuBurst[j] == 0) {
                comp_pro++;
            }
        }
        if (comp_pro == TOTAL_PROCESS) {
            break;
        }
    }
    printStatistics();
}

void SRBF() {

    int quantum = 0;
    printf("Please enter a time quantum.\n");
    scanf("%d", &quantum);

    current_time = 0;
    int cpuBurst[TOTAL_PROCESS];

    int p = 0;
    for (; p < TOTAL_PROCESS; p++) {
        cpuBurst[p] = processTable[p]->cpu_burst_time;
    }
    printf("----------------------------------------------------\n");
    printf("       SHORTEST REMAINING BURST FIRST SCHEDULING..\n");
    printf("----------------------------------------------------\n");

    while (1) {
        int t = 0;
        int min_burst = 1000;
        int i = 0;
        int comp_pro = 0;

        for (; t < TOTAL_PROCESS; t++) {
            if (current_time < processTable[t]->arrival_time) {
                continue;
            }

            if (cpuBurst[t] == 0) {
                comp_pro++;
                continue;
            }

            if (cpuBurst[t] <= min_burst) {
                min_burst = cpuBurst[t];
                i = t;
            }
        }

        if (comp_pro == TOTAL_PROCESS) {
            break;
        }

        printf("Process %s , cpu Burst %d", processTable[i]->process_name, cpuBurst[i]);

        char *name = processTable[i]->process_name;
        printf("[%2d - %-2d] %5s running.\n", current_time,
               ((cpuBurst[i] > quantum) ? (current_time + quantum) : (current_time + cpuBurst[i])), name);

        if (cpuBurst[i] > quantum) {
            cpuBurst[i] -= quantum;
            current_time += quantum;
        } else {
            current_time += cpuBurst[i];
            cpuBurst[i] = 0;

            processTable[i]->end_time = current_time;
            processTable[i]->turn_around_time = processTable[i]->end_time - processTable[i]->arrival_time;
            processTable[i]->wait_time = processTable[i]->turn_around_time - processTable[i]->cpu_burst_time;
        }
    }
    printStatistics();
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Please provide a input process table as a txt file.\n");
        return 1;
    }

    readProcessTable(argv[1]);
    printProcessTable();
    int choice = 0;
    do {
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n"
               "1. First Come First Served. (FCFS)\n"
               "2. Round Robin. (RR)\n"
               "3. Shortest Remaining Burst First. (SRBF)\n"
               "4. Exit.\n");

        scanf("%d", &choice);
        switch (choice) {
            case 1:
                FCFS();
                break;
            case 2:
                RR();
                break;
            case 3:
                SRBF();
                break;
            case 4:
                printf("Exiting.\n");
                return 1;
        }
    } while (choice != 4);

}