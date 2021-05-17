#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct mymsgbuf // Custom structure for the message
{
    long mtype;
    struct {
        double mvalue;
        long msender;
    } info;
} mybuf;

void send_request(int msqid, double value, long sender) {
    mybuf.mtype = 1;
    mybuf.info.mvalue = value;
    mybuf.info.msender = sender;
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf*)&mybuf, sizeof(mybuf.info), 0) < 0) {
        printf("Can\'t send request to server\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
}

void receive_response(int msqid, long client) {
    if (msgrcv(msqid, (struct msgbuf*)&mybuf, sizeof(mybuf.info), client, 0) < 0) {
        printf("Can\'t receive response from server\n");
        exit(-1);
    }
}

int main(void)
{
    int msqid;
    char pathname[] = "lab11-3-server.c";
    key_t  key;
    long pid;
    double value;
    int scanf_result;
    char garbage[50];

    pid = (long)getpid();

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666)) < 0) {
        printf("Can\'t get connect to server\n");
        exit(-1);
    }

    while (1) {
        printf("Please, enter floating point value:\n");
        scanf_result = scanf("%lf", &value);

        if (scanf_result <= 0) {
            printf("Wrong input\n");
            scanf("%s", garbage);
            continue;
        }

        // Using pid as message type
        send_request(msqid, value, pid);

        if (value == 0) {
            exit(0);
        }

        receive_response(msqid, pid);

        printf("Server responded with value %lf\n", mybuf.info.mvalue);
    }

    return 0;
}