#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "headers/semaphore.h"
#include "headers/producer.h"
#include "headers/consumer.h"

#define BUFFER_SIZE STACK_LENGTH

int main()
{
    int sh_sem_id, sh_empt_id, sh_full_id, sh_buff_id;
    pid_t pid0, pid1, pid2;

    sh_sem_id = shmget(IPC_PRIVATE, sizeof(Semaphore), 0666);
    if (sh_sem_id == -1) perror("Error creating shared memory segment for mutex semaphore.\n");
    else printf("Created shared memory segment for mutex semaphore.\n");

    sh_empt_id = shmget(IPC_PRIVATE, sizeof(Semaphore), 0666);
    if (sh_empt_id == -1) perror("Error creating shared memory segment for empty semaphore.\n");
    else printf("Created shared memory segment for empty semaphore.\n");

    sh_full_id = shmget(IPC_PRIVATE, sizeof(Semaphore), 0666);
    if (sh_full_id == -1) perror("Error creating shared memory segment for full semaphore.\n");
    else printf("Created shared memory segment for full semaphore.\n");

    sh_buff_id = shmget(IPC_PRIVATE, sizeof(Stack), 0666);
    if (sh_buff_id == -1) perror("Error creating shared memory for stack head.\n");
    else printf("Created shared memory for stack head.\n");

    pid1 = fork();

    if (pid1 < 0)
    {
        printf("Process P0 failed to spawn child P1-producer. Exiting.\n");
        exit(1);
    }
    if (pid1 > 0)
    {
        // P0
        pid2 = fork();
        if (pid2 < 0)
        {
            printf("Process P0 failed to spawn child P2-consumer. Exiting.\n");
        }
        if (pid2 > 0)
        {
            // P0
        }
        else 
        {
            // P2-consumer
            Semaphore* s;
            s = (Semaphore*)shmat(sh_sem_id, NULL, 0);
            if ((int)s == -1) perror("Failed to attach mutex semaphore memory address in process P2-consumer.\n");
            else printf("Attached mutex semaphore memory address in process P2-consumer.\n");

            Semaphore* empty;
            empty = (Semaphore*)shmat(sh_empt_id, NULL, 0);
            if ((int)empty == -1) perror("Failed to attach empty semaphore memory address in process P2-consumer.\n");
            else printf("Attached empty semaphore memory address in process P2-consumer.\n");

            Semaphore* full;
            full = (Semaphore*)shmat(sh_full_id, NULL, 0);
            if ((int)full == -1) perror("Failed to attach full semaphore memory address in process P2-consumer.\n");
            else printf("Attached full semaphore memory address in process P2-consumer.\n");

            Stack* st_ptr;
            st_ptr = (Stack*)shmat(sh_buff_id, NULL, 0);
            if ((int)st_ptr == -1) perror("Failed to attach stack head memory address in process P2-consumer.\n");
            else printf("Attached stack head memory address in process P2-consumer.\n");
            
            sem_mutex_init(s);
            sem_empty_init(empty, BUFFER_SIZE);
            sem_full_init(full);
            stack_init(st_ptr);
            int item;

            for (int i = 0; i < 11; i++)
            {
                down(full);
                down(s);
                item = remove_item(st_ptr);
                printf("P2-consumer removed item.\n");
                up(s);
                up(empty);
                consume_item(item);
            }
        }
    }
    else
    {
        // P1-producer
        Semaphore* s;
        s = (Semaphore*)shmat(sh_sem_id, NULL, 0);
        if ((int)s == -1) perror("Failed to attach mutex semaphore memory address in process P1-producer.\n");
        else printf("Attached mutex semaphore memory address in process P1-producer.\n");

        Semaphore* empty;
        empty = (Semaphore*)shmat(sh_empt_id, NULL, 0);
        if ((int)empty == -1) perror("Failed to attach empty semaphore memory address in process P1-producer.\n");
        else printf("Attached empty semaphore memory address in process P1-producer.\n");

        Semaphore* full;
        full = (Semaphore*)shmat(sh_full_id, NULL, 0);
        if ((int)full == -1) perror("Failed to attach full semaphore memory address in process P1-producer.\n");
        else printf("Attached full semaphore memory address in process P1-producer.\n");

        Stack* st_ptr;
        st_ptr = (Stack*)shmat(sh_buff_id, NULL, 0);
        if ((int)st_ptr == -1) perror("Failed to attach stack head memory address in process P1-producer.\n");
        else printf("Attached stack head memory address in process P1-producer.\n");

        int item;
        for (int i = 0; i < 11; i++)
        {
            item = produce_item();
            down(empty);
            down(s);
            insert_item(item, st_ptr);
            printf("P1-producer inserted item.\n");
            up(s);
            up(full);
        }
    }
    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
}