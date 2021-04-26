#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
    int msqid;      // IPC descriptor for the message queue
    char pathname[] = "lab11-2a.c"; // The file name used to generate the key.
                               // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int i, len;      // Cycle counter and the length of the informative part of the message

    struct mymsgbuf { // Custom structure for the message
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
    //
    // Trying to get access by key to the message queue, if it exists,
    // or create it, with read & write access for all users.
    //
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    printf("A start.\n");

    const char  chars[] = { 'a', 'b', 'c', 'd', 'e' };
    const int   nums[] = { 1, 2, 3, 4, 5 };

    /* Send information */

    for (i = 0; i < 5; i++) {
        mybuf.mtype = 1;
        mybuf.info.cinfo = chars[i];
        mybuf.info.iinfo = nums[i];
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }


        len = sizeof(mybuf.info);

        if ((len = msgrcv(msqid, (struct msgbuf*)&mybuf, len, 2, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        printf("A received: message type = %ld, char = %c, number = %d\n", mybuf.mtype, mybuf.info.cinfo, mybuf.info.iinfo);
    }

    /* Send the last message */

    mybuf.mtype = LAST_MESSAGE;

    mybuf.info.cinfo = 'q';
    mybuf.info.iinfo = -1;

    if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }

    printf("A end.\n");

    return 0;
}