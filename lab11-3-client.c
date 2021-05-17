#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**
 *  Client
 */
struct client
{
    long mtype;
    struct
    {
        pid_t pid;
        float message;
    } info;
} clientbuf;

struct server
{
    long mtype;
    struct {
        float message;
    } info;
} serverbuf;


int main(void)
{
    int msqid;
    char pathname[] = "lab11-3-server.c";
    key_t  key;
    int len, maxlen;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    // Инициализируем клиента
    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();

    float msg;
    while (1)
    {
        printf("Enter float: ");
        scanf("%f", &msg);
        clientbuf.info.message = msg;

        printf("Client send type = %d message = '%f'\n", clientbuf.mtype, clientbuf.info.message);

        len = sizeof(clientbuf.info);
        if (msgsnd(msqid, &clientbuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }

        printf("Waiting for server...\n");

        if (msgrcv(msqid, &serverbuf, sizeof(serverbuf.info), getpid(), 0) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        printf("Server answered: %f\n", serverbuf.info.message);
    }

    return 0;
}