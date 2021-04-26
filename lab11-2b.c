#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[] = "lab11-2a.c";
    key_t  key;
    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            char cinfo;
            int iinfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    const char  chars[] = { 'a', 'b', 'c', 'd', 'e' };
    const int   nums[] = { 1, 2, 3, 4, 5 };

    int i = -1;
    while (1) {
        ++i;
        maxlen = sizeof(mybuf.info);

        if ((len = msgrcv(msqid, (struct msgbuf*)&mybuf, maxlen, 1, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        printf("B received: message type = %ld, char = %c, number = %d\n", mybuf.mtype, mybuf.info.cinfo, mybuf.info.iinfo);


        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL); //remove when LAST_MESSAGE TYPE is recieved
            exit(0);
        }

        mybuf.mtype = 2;
        mybuf.info.cinfo = chars[i];
        mybuf.info.iinfo = nums[i];
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }
    }

    return 0;
}