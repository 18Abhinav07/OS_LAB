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
    int arrived;
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
        data->arrived = 0;
        processTable[TOTAL_PROCESS] = data;
        TOTAL_PROCESS += 1;
    }

    fclose(file);
    return;
}

void printProcessTable(FILE *res) {
    int i = 0;
    for (i; i < TOTAL_PROCESS; i++) {
        fprintf(res, "process_name: %s, arrival_time: %d, cpu_burst_time: %d\n", processTable[i]->process_name,
                processTable[i]->arrival_time,
                processTable[i]->cpu_burst_time);
    }
}

void printStatistics(FILE *res) {
    int i = 0;
    float avg_turn_around_time = 0.00;
    float avg_wait_time = 0.00;
    fprintf(res, "%-20s", "Turnaround Times:");
    for (i; i < TOTAL_PROCESS; i++) {
        fprintf(res, "%s[%d], ", processTable[i]->process_name, processTable[i]->turn_around_time);
        avg_turn_around_time += processTable[i]->turn_around_time;
    }
    fprintf(res, "\n");

    i = 0;
    fprintf(res, "%-20s", "Wait Times:");
    for (i; i < TOTAL_PROCESS; i++) {
        fprintf(res, "%s[%d], ", processTable[i]->process_name, processTable[i]->wait_time);
        avg_wait_time += processTable[i]->wait_time;
    }
    fprintf(res, "\n");

    fprintf(res, "Average turnaround time: %.2f\nAverage wait time: %.2f \n", (avg_turn_around_time / (TOTAL_PROCESS)),
            (avg_wait_time / (TOTAL_PROCESS)));
}

void FCFS(FILE *res) {
    current_time = 0;
    int i = 0;
    fprintf(res, "----------------------------------------------------\n");
    fprintf(res, "         FIRST COME FIRST SERVE SCHEDULING.\n");
    fprintf(res, "----------------------------------------------------\n");
    for (i; i < TOTAL_PROCESS; i++) {
        int cpu_time = processTable[i]->cpu_burst_time;
        char *name = processTable[i]->process_name;

        processTable[i]->wait_time = current_time - processTable[i]->arrival_time;
        processTable[i]->end_time = current_time + cpu_time;
        processTable[i]->turn_around_time = processTable[i]->end_time - processTable[i]->arrival_time;

        fprintf(res, "[%2d - %-2d] %5s running.\n", current_time, processTable[i]->end_time, name);
        current_time += cpu_time;
    }
    printStatistics(res);
    return;
}


void RR(FILE *res) {

    int quantum = 0;
    printf("Please enter a time quantum.\n");
    scanf("%d", &quantum);

    current_time = 0;
    int cpuBurst[TOTAL_PROCESS];

    int i = 0;
    for (; i < TOTAL_PROCESS; i++) {
        cpuBurst[i] = processTable[i]->cpu_burst_time;
    }

    fprintf(res, "----------------------------------------------------\n");
    fprintf(res, "               ROUND ROBIN SCHEDULING..\n");
    fprintf(res, "----------------------------------------------------\n");

    int queue[TOTAL_PROCESS];
    int f = 0;
    int r = 0;

    queue[r] = 0;
    processTable[0]->arrived = 1;

    while (1) {
        int comp = 0;
        int itr = 0;
        for (itr; itr < TOTAL_PROCESS; itr++) {
            if (cpuBurst[itr] == 0) {
                comp++;
            }
        }

        if (comp == TOTAL_PROCESS) {
            break;
        }


        int i = queue[f];
        f = (f + 1) % TOTAL_PROCESS;
        if (cpuBurst[i] == 0) {
            continue;
        }

        char *name = processTable[i]->process_name;
        fprintf(res, "[%2d - %-2d] %5s running.\n", current_time,
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
        int ind = 0;
        for (; ind < TOTAL_PROCESS; ind++) {
            if (processTable[ind]->arrived == 0 && processTable[ind]->arrival_time <= current_time) {
                r = (r + 1) % TOTAL_PROCESS;
                queue[r] = ind;
                processTable[ind]->arrived = 1;
            }
        }

        if (cpuBurst[i] != 0) {
            r = (r + 1) % TOTAL_PROCESS;
            queue[r] = i;
        }
    }
    printStatistics(res);
}

void SRBF(FILE *res) {


    current_time = 0;
    int cpuBurst[TOTAL_PROCESS];

    int p = 0;
    for (; p < TOTAL_PROCESS; p++) {
        cpuBurst[p] = processTable[p]->cpu_burst_time;
    }
    fprintf(res, "----------------------------------------------------\n");
    fprintf(res, "       SHORTEST REMAINING BURST FIRST SCHEDULING..\n");
    fprintf(res, "----------------------------------------------------\n");

    int comp = 0;
    int start = 0;
    int cur_pro = 0;

    while (1) {
        cpuBurst[cur_pro] -= 1;
        current_time += 1;

        if (cpuBurst[cur_pro] == 0) {
            processTable[cur_pro]->end_time = current_time;
            processTable[cur_pro]->turn_around_time =
                    processTable[cur_pro]->end_time - processTable[cur_pro]->arrival_time;
            processTable[cur_pro]->wait_time =
                    processTable[cur_pro]->turn_around_time - processTable[cur_pro]->cpu_burst_time;
            comp += 1;
        }

        if (comp == TOTAL_PROCESS) {
            char *name = processTable[cur_pro]->process_name;
            fprintf(res, "[%2d - %-2d] %5s running.\n", start, current_time, name);
            break;
        }


        int ind = 0;
        int shortest_pro = 0;
        int min = 9999;

        for (; ind < TOTAL_PROCESS; ind++) {
            if (cpuBurst[ind] != 0 && processTable[ind]->arrival_time <= current_time) {
                if (cpuBurst[ind] <= min) {
                    min = cpuBurst[ind];
                    shortest_pro = ind;
                }
            }
        }

        printf(" current -> %d\n", cur_pro);
        printf(" next shortest -> %d\n", shortest_pro);

        if (shortest_pro != cur_pro) {
            char *name = processTable[cur_pro]->process_name;
            fprintf(res, "[%2d - %-2d] %5s running.\n", start, current_time, name);
            cur_pro = shortest_pro;
            start = current_time;
        }
    }
    printStatistics(res);
}

int main(int argc, char *argv[]) {

    FILE *res = fopen("result.txt", "a");

    if (argc <= 1) {
        printf("Please provide a input process table as a txt file.\n");
        return 1;
    }

    readProcessTable(argv[1]);
    printProcessTable(res);

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
                FCFS(res);
                break;
            case 2:
                RR(res);
                break;
            case 3:
                SRBF(res);
                break;
            case 4:
                printf("Exiting.\n");
                return 1;
        }
    } while (choice != 4);

}
