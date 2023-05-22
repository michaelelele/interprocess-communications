#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    int dividend, divisor, fd, sharedMemoryID;

    if(argc != 3){ //Checks if invalid number of arguments was sent
        fprintf(stderr, "Checker Error: Invalid number of arguments.\n");
        exit(1);
    }

    divisor = atoi(argv[0]);
    dividend = atoi(argv[1]);
    fd = atoi(argv[2]);
    sharedMemoryID = atoi(argv[2]);

    pid_t pid = getpid();

    read(fd, &sharedMemoryID, sizeof(sharedMemoryID));

    int *sharedMemoryPointer = (int*)shmat(sharedMemoryID, NULL, 0);
    printf("Checker process [%d]: starting.\n", pid);
    printf("Checker process [%d]: read %d bytes containing shm ID %d\n", pid, sizeof(sharedMemoryID), sharedMemoryID);

    if(dividend % divisor == 0){ //Returns 1 if dividend is divisible by divisor and 0 if it doesn't
        *sharedMemoryPointer = 1;
        printf("Checker Process [%d]: %d *IS* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: wrote result (%d) to shared memory\n", pid, *sharedMemoryPointer);
        shmdt(sharedMemoryPointer);
        return 0;
    }
    else{
        *sharedMemoryPointer = 0;
        printf("Checker Process [%d]: %d *IS NOT* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: wrote result (%d) to shared memory\n", pid, *sharedMemoryPointer);
        shmdt(sharedMemoryPointer);
        return 0;
    }

}
