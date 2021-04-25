#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
int main()
{
    /* »нициализируем семафор и сразу увеличиваем на 1 (можно считать, что инициализируем с 1)
     * –одитель: уменьшаем семафор на 1 (D(S, 1))
     * записываем (write), ”величиваем на 2 (A(S, 2)), ждем пока будет 0 (Z(S)), уменьшаем на 1 (D(S, 1)), читаем (read).
     * –ебенок: уменьшаем на 2 (D(S,2)), читаем (read), записываем(write), увеличиваем на 1(A(S,1)).
     */
    int     fd[2], result;
    int N;
    key_t key;
    char pathname[] = "lab10.c";
    int semid;
    struct sembuf buf;
    size_t size;
    char  resstring[14];

    printf("Enter N>1: \n");

    scanf("%d", &N);

    if (N < 2) {
        printf("N should be >1.\n");
        exit(-1);
    }
    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can not create new semaphore\n");
        exit(-1);
    }

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;

    if (semop(semid, &buf, 1) < 0) {
        printf("Can not wait for condition\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);

    }
    else if (result > 0) {
        /* Parent process */

        buf.sem_num = 0;
        buf.sem_op = -1;
        buf.sem_flg = 0;

        if (semop(semid, &buf, 1) < 0) {
            printf("Can not wait for condition\n");
            exit(-1);
        }

        for (int i = 0; i < N; i++) {
            size = write(fd[1], "Hello, world!", 14);

            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            buf.sem_num = 0;
            buf.sem_op = 2;
            buf.sem_flg = 0;

            if (semop(semid, &buf, 1) < 0) {
                printf("Can not wait for condition\n");
                exit(-1);
            }

            buf.sem_num = 0;
            buf.sem_op = 0;
            buf.sem_flg = 0;

            if (semop(semid, &buf, 1) < 0) {
                printf("Can not wait for condition\n");
                exit(-1);
            }

            buf.sem_num = 0;
            buf.sem_op = -1;
            buf.sem_flg = 0;

            if (semop(semid, &buf, 1) < 0) {
                printf("Can not wait for condition\n");
                exit(-1);
            }
            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Can not read string from pipe\n");
                exit(-1);
            }
            printf("Parent read: %s\n", resstring);
        }
        printf("Parent exit\n");
    }
    else {
        /* Child process */
        for (int i = 0; i < N; i++) {
            buf.sem_num = 0;
            buf.sem_op = -2;
            buf.sem_flg = 0;

            if (semop(semid, &buf, 1) < 0) {
                printf("Can not wait for condition\n");
                exit(-1);
            }

            size = read(fd[0], resstring, 14);

            if (size < 0) {
                printf("Child Can not read string from pipe\n");
                exit(-1);
            }
            printf("child read: %s\n", resstring);
            size = write(fd[1], "World, hello!", 14);
            if (size < 14) {
                printf("Child Can not write string to pipe\n");
                exit(-1);
            }
            buf.sem_num = 0;
            buf.sem_op = 1;
            buf.sem_flg = 0;
            if (semop(semid, &buf, 1) < 0) {
                printf("Can not wait for condition\n");
                exit(-1);
            }
        }
        if (close(fd[0]) < 0) {
            printf("child: Can\'t close reading side of pipe\n"); exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("child: Can\'t close writing side of pipe\n"); exit(-1);
        }
        printf("Child exit");
    }
    return 0;
}