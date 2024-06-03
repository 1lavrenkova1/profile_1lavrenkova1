#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define KEY "mails"

int queue;

typedef struct {
    long type;
    char buf[255];
} Message;

void hnd(int sig) {
    signal(sig, SIG_DFL);
    if(msgctl(queue, IPC_RMID, 0) < 0) {
        printf("Can't delete queue\n");
        exit(1);
    }
}

int main() {
    char keyName[255] = {0};
    strcpy(keyName, KEY);
    
    key_t key = ftok(keyName, 'Q'); 
    if(key == -1) {
        printf("can't get key for the file %s\n", keyName);
        return 1;
    }

    queue = msgget(key, IPC_CREAT | 0666);
    if(queue < 0) {
        printf("Can't create queue\n");
        return 2;
    }
    signal(SIGINT, hnd);

    Message msg; 
    int res;
    for(;;) {   
        bzero(msg.buf, 255);
        fgets(msg.buf, 255, stdin);
        msg.buf[strlen(msg.buf) - 1] = '\0';

        msg.type = 2L;
        res = msgsnd(queue, (void*)&msg, sizeof(Message), 0);
        if(res != 0) {
            printf("error while sending message\n");
            kill(getpid(), SIGINT);
        }
    }

    return 0;
}