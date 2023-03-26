#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>


void printArray(int array[3][3], int size) {
    sleep(1);
    printf("Salam");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    printf("Bye");
    
    printf("\n\n");
}

int main(void) {
    int array[3][3] = {{0, 0, 0},
                      {0, 0, 0},
                      {0, 0, 0}};
    int N = 3;
    // printArray(array, N);

    for (int i = 0; i < N; i++) {
        pid_t pid = getpid();
        for (int j = 0; j < N - 1; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                array[i][j] = 1;
                break;
            }
        }
        if (getpid() == pid) {
            array[i][N - 1] = 1;
        }
    }

    printArray(array, N);

}