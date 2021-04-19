#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>


int dec(int semid, struct sembuf* buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int inc(int semid, struct sembuf* buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int main()
{
    int parent[2], result;
    size_t size = 0;
    key_t key;
    char pathname[] = "lab10.c";
    struct sembuf buffer;

    int semid;
    char resstring[14];

    if (pipe(parent) < 0) {
        printf("Cannot open parent\'s pipe\n\r");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not found, trying to create one \n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Cannon get Semid\n");
            exit(-1);
        }
        printf("Semaphore created succesfully\n");
    }


    result = fork();

    if (result < 0) {
        printf("Cannot fork child\n\r");
        exit(-1);
    }

    else if (result > 0) {
        int N;
        printf("Enter N>1: \n");
        scanf("%d", &N);
        if (N < 2) {
            printf("N should be >1\n");
            exit(-1);
        }

        for (size_t i = 0; i < N; i++)
        {
            if (write(parent[1], "Hello, world!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }

            printf("Pair #%d, Parent has sent a message\n\r", i + 1);

            inc(semid, &buffer);
            dec(semid, &buffer);

            size = read(parent[0], resstring, 14);

            if (size != 14) {
                printf("Can\'t read from child\n\r");
                exit(-1);
            }

            printf("Parent recieved message: %s\n\r", resstring);
        }
        close(parent[0]);
    }
    else {
        int counter = 0;
        while (1) {
            dec(semid, &buffer);
            size = read(parent[0], resstring, 14);

            if (size < 0) {
                close(parent[1]);
                close(parent[0]);

                printf("It is over\n");
                return 0;
            }

            printf("Pair #%d, Child recieved message: %s\n\r", ++counter, resstring);

            if (write(parent[1], "Hi, my parent", 14) != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            inc(semid, &buffer);
        }
    }
    return 0;
}