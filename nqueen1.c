#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <time.h>


void printArray(size_t size ,int array[size][size]) {
    // sleep(1);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }    
}


bool isSafe(size_t size, int array[size][size], int row, int column) {
    for (int i = row - 1; i >= 0; i--) {
        if (array[i][column]) {
            return false;
        }
    }

    for (int i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
        if (array[i][j]) {
            
            return false;
        }
    }

    for (int i = row - 1, j = column + 1; i >= 0 && j <= size - 1; i--, j++) {
        if (array[i][j]) {
            
            return false;
        }
    }
    
    return true;
}

int main(void) {
    int N;
    scanf("%d", &N);
    printf("*************\n");
    // pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    // pipe

    // array declaration
    int array[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array[i][j] = 0;
        }
    }
    // array declared

 
    for (int i = 0; i < N; i++) {
        pid_t pid = getpid();
        for (int j = 0; j < N - 1; j++) {
            clock_t t1 = clock();
            pid_t pid2 = fork();
            if (pid2 > 0) {
                int status;
                waitpid(pid2, &status, 0);
                clock_t t2 = clock();

                int terminated_pid;
                read(pipefd[0], &terminated_pid, sizeof(int));
                if (terminated_pid != -1) {
                    double time_taken = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
                    printf("Successful child process with PID = %d took %f second to execute\n", terminated_pid, time_taken);
                }
            }

            if (pid2 == 0) {
                array[i][j] = 1;
                // put checkArray here
                if (!isSafe(N, array, i, j)) {
                    int unsuccess_pid = -1;
                    write(pipefd[1], &unsuccess_pid, sizeof(int));
                    exit(EXIT_SUCCESS);
                }

                break;
            }
        }
        if (getpid() == pid) {
            array[i][N - 1] = 1;
            if (!isSafe(N, array, i, N - 1)) {
                int unsuccess_pid = -1;
                write(pipefd[1], &unsuccess_pid, sizeof(int));
                exit(EXIT_SUCCESS);
            }

        }
    }

    pid_t success_pid = getpid();
    write(pipefd[1], &success_pid, sizeof(int));
    printArray(N, array);

}