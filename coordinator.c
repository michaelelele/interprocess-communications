#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[]){
    int divisor, dividend;
    divisor = atoi(argv[1]);

    pid_t pid[argc - 2];
    int size_pid[4];
    int sharedMemoryID[argc - 2];
    //pid = fork();


    if (argc != 6) { //Sends error if invalid number of arguments sent.
        fprintf(stderr, "Coordinator Error: Invalid number of arguments.\n");
        exit(1);
    }


    for(int i = 2; i < argc; i++){
        dividend = atoi(argv[i]);

        int fd[2];
        pipe(fd);

        sharedMemoryID[i - 2] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

        pid[i - 2] = fork();


        if(pid[i - 2] == -1){ //Send error if fork fails
            fprintf(stderr, "Coordinator Error: Fork failed.\n");
        }

        else if(pid[i - 2] == 0){ // Creates child process
            //printf("Checker process [%d]: starting.\n",getpid());
            char buffer[8];
            sprintf(buffer, "%d", fd[0]);
            execlp("checker", argv[1], argv[i], buffer); //Calls and passes arguments to checker program
        }

        else{
            printf("Coordinator: forked process with ID %d.\n", pid[i - 2]); //Prints that child process was created
            printf("Coordinator: wrote shm ID %d to pipe (%d bytes)\n", sharedMemoryID[i - 2], sizeof(sharedMemoryID[i - 2]));

            size_pid[i-2] = pid[i-2];

            close(fd[0]);
            write(fd[1], &sharedMemoryID[i - 2], sizeof(sharedMemoryID[i - 2]));
            close(fd[1]);

        }

    }

    for(int i = 0; i < argc -2; i++){
        dividend = atoi(argv[i+2]);
        printf("Coordinator: waiting on process ID %d...\n", size_pid[i]);
        wait(&pid[i]); //Waits
        int *sharedMemoryPointer = (int*)shmat(sharedMemoryID[i], NULL, 0);
        int result = *sharedMemoryPointer;

        shmctl(*sharedMemoryPointer, IPC_RMID, NULL);

        if(result == 1){ //Prints if dividend is divisble by divisor
            printf("Coordinator: result %d read from shared memory: %d is divisible by %d.\n", result, dividend, divisor);
        }

        else{ //Prints if dividend is not divisble by divisor
            printf("Coordinator: result %d read from shared memory: %d is not divisible by %d.\n", result, dividend, divisor);
        }
    }

    printf("Coordinator: exiting.\n"); //Prints exit line
    return 0;

}