#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

/**
 *  Server
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

    int pidFile = open("lab11-3-server.c", O_CREAT | O_RDWR, 0666);
    int rc = flock(pidFile, LOCK_EX | LOCK_NB);
    if (!rc) {
        if ((key = ftok(pathname, 0)) < 0) {
            printf("Can\'t generate key\n");
            exit(-1);
        }

        if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get msqid\n");
            exit(-1);
        }

        while (1) {
            if (msgrcv(msqid, (struct clientbuf*)&clientbuf, sizeof(clientbuf.info), -2, 0) < 0) {
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }

            if (clientbuf.mtype == 2) {
                printf("Server killed\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
                exit(-1);
            }

            printf("Client pid = %d sent: %f\n", clientbuf.info.pid, clientbuf.info.message);

            serverbuf.mtype = clientbuf.info.pid;
            serverbuf.info.message = clientbuf.info.message * clientbuf.info.message;

            if (msgsnd(msqid, &serverbuf, sizeof(serverbuf.info), 0) < 0) {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
                exit(-1);
            }

            printf("Sending answer to client.\n");

        }
    }
    else {
        printf("Server is running.\n");
    }
    return 0;
}