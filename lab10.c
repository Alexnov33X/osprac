#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{



    int     fd[2], result;
    int     communications_count; 
    key_t   key;                  
    int     semid;                
    struct  sembuf mybuf;         
    char    pathname[] = "lab10.c";
    size_t  size;
    char    resstring[15];
    printf("Enter N>1: \n");
  
    scanf("%d", &communications_count);

    if (communications_count < 2) {
        printf("N should be >1.\n");
        exit(-1);
    }

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Impossible to generate the key.\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Error: impossible to create the semaphore.\n");
        exit(-1);
    }

    mybuf.sem_num = 0;
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;

    if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t set initial semaphore value as 1.\n");
        exit(-6);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork the child. Terminating.\n");
        exit(-5);
    }
    else if (result > 0) {
        mybuf.sem_num = 0;
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;

        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t decrease semaphore value by 1.\n");
            exit(-1);
        }

        for (int i = 0; i < communications_count; ++i) {
            size = write(fd[1], "Hello, child!", 14);

            if (size != 14) {
                printf("Parent: unable to write all the chars to the pipe.\n");
                exit(-1);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = 2;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in parent-program.\n");
                exit(-1);
            }
            mybuf.sem_num = 0;
            mybuf.sem_op = 0;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in parent-program.\n");
                exit(-6);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in parent-program.\n");
                exit(-6);
            }

            size = read(fd[0], resstring, 15);

            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }

            printf("Parent: read the string: %s\n", resstring);
        }


        printf("Parent finished its job.\n");

    }
    else {
        for (int i = 0; i < communications_count; ++i) {

            mybuf.sem_num = 0;
            mybuf.sem_op = -2;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly.\n");
                exit(-6);
            }

            size = read(fd[0], resstring, 14);

            if (size < 0) {
                printf("Child: Impossible to read the string from the pipe.\n");
                exit(-1);
            }

            printf("Child: read the string: %s\n", resstring);

            size = write(fd[1], "Hello, parent!", 15);

            if (size != 15) {
                printf("Child: unable to write all the chars to the pipe.\n");
                exit(-1);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = 1;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly.\n");
                exit(-6);
            }
        }
        if (close(fd[0]) < 0) {
            printf("Child: Can\'t close reading side of pipe\n");
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Child: Can\'t close writing side of pipe\n");
            exit(-1);
        }
    }

    return 0;
}