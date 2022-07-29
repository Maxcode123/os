#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>

/*
Testing IPC with shared memory segments.
Process P0 has one kid process, namely P1.
P1 creates a pointer to the shared memory address and assigns a value to it.
P0 waits for P1 to finish, creates a pointer to the shared memory address and increments the value.
The final value is that assigned by P1 plus the increment that P0 did.
*/

int main()
{
    int madd_id;
    pid_t pid0, pid1;

    madd_id = shmget(IPC_PRIVATE, sizeof(int), 0666);
    if (madd_id == -1) perror("Error creating shared memory address.\n");
    else printf("Created shared memory address.\n");

    pid1 = fork();

    if (pid1 < 0)
    {
        printf("Process P0 failed to spawn child P1. Exiting.\n");
        exit(1);
    }
    if (pid1 == 0)
    {
        // P1
        int* ptr1;
        ptr1 = (int*)shmat(madd_id, NULL, 0);
        if ((int)ptr1 == -1) perror("Failed to attach memory address in process P1.\n");
        else printf("Attached memory address in process P1.\n");
        *ptr1 = 10;
        int err1;
        err1 = shmdt((void*)ptr1);
        if (err1 == -1) perror("Failed to detach memory address in process P1.\n");
        else printf("Detached memory address in process P1.\n");
    }
    else
    {
        // P0
        int status;
        wait(&status);

        int* ptr0;
        ptr0 = (int*)shmat(madd_id, NULL, 0);
        if ((int)ptr0 == -1) perror("Failed to attach memory address in process P0.\n");
        else printf("Attached memory address in process P0.\n");
        
        int a;
        a = *ptr0 + 5;

        int err0;
        err0 = shmdt(ptr0);
        if (err0 == -1) perror("Failed to detach memory address in process P0.\n");
        else printf("Detached memory address in process P0.\n");

        printf("Value of shared int now is : %d\n", a);

        int err;
        err = shmctl(madd_id, IPC_RMID, 0);
        if (err == -1) perror("Failed to remove shared memory segment in process P0\n");
        else printf("Removed shared memory segment in process P0\n");
    }
}