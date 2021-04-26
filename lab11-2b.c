#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

int main() {
    int msqid;

    char pathname[] = "lab11-2a.c";

    key_t key;

    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Cannot generate key\n");
        return -1;
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Cannot get msqid\n");
        return -1;
    }

    printf("program 2 start receiving\n");
    for (int i = 0; i < 5; ++i) {
        maxlen = sizeof(mybuf.info);
        if (msgrcv(msqid, (struct msgbuf*)&mybuf, maxlen, 1, 0) < 0) {
            printf("Cannotreceive message from queue\n");
            return -1;
        }

        printf("program 2 received: type = %ld, sInfo = %i\n", mybuf.mtype, mybuf.info.sinfo);
    }

    printf("program 2 finished receiving\n");
    printf("program 2 start sending\n");

    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 2;
        mybuf.info.sinfo = 2222 + i;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
            printf("Cannot send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            return -1;
        }
    }

    printf("program 2 finished sending\n");

    return 0;
}