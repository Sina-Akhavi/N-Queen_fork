#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>


void printArray(int array[5][5], int size) {
    sleep(1);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }    
    printf("\n\n");
}

bool isSafe(int array[5][5], int size, int row, int column) {
    for (int i = row - 1; i >= 0; i--) {
        if (array[i][column]) {
            return false;
        }
    }

    for (int i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
        if (array[i][j]) {
            // printf("right here\n");
            return false;
        }
    }

    for (int i = row - 1, j = column + 1; i >= 0 && j <= size - 1; i--, j++) {
        if (array[i][j]) {
            // printf("right there\n");
            return false;
        }
    }
    
    return true;
}

int main(void) {
    int array[5][5] = {{0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0 ,0 ,0 ,0}};
    int N = 5;

    // printArray(array, N);

    for (int i = 0; i < N; i++) {
        pid_t pid = getpid();
        for (int j = 0; j < N - 1; j++) {
            pid_t pid2 = fork();
            if (pid2 > 0) {
                int status;
                waitpid(pid2, &status, 0);
            }
            if (pid2 == 0) {
                array[i][j] = 1;
                // put checkArray here
                if (!isSafe(array, N, i, j)) {
                    exit(EXIT_SUCCESS);
                }

                break;
            }
        }
        if (getpid() == pid) {
            array[i][N - 1] = 1;
            if (!isSafe(array, N, i, N - 1)) {
                exit(EXIT_SUCCESS);
            }

        }
    }

    printArray(array, N);

}