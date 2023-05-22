# interprocess-communications
InterProcess Communications using Shared Memory and Pipes

The checker accepts two arguments, the dividend and divisor, checks if the dividend is divisible by the divisor, and prints the result.

The coordinator spawns 4 processes using the fork() command and ensures that it completes a full cycle of fork(), exec(), and wait() before spawning a new process.

The coordinator then prints out the process ID of the process created and then executes the exec() command to load the checker file into the newly created process with both the dividend and divisor. 

The controller creates shared memory and pipe. The pipe FD is passed via command lind to the checker and the segment ID is passed via the pipe to the checker. The resulst computed by the checker are written to shared memory and read by the coordinator

To compile the program, a make file is provided

