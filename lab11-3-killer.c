#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**
 *  Killer
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

    if ((msqid = msgget(key, 0666)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    clientbuf.mtype = 2;
    clientbuf.info.message = 0;
    clientbuf.info.pid = getpid();

    printf("Kiling server....");

    len = sizeof(clientbuf.info);
    if (msgsnd(msqid, &clientbuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }


    return 0;
}