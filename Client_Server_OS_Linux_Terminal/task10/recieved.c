#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY "mails"

typedef struct {
    long type;
    char buf[255];
} Message;

int main() {
    char keyName[255] = {0};
    strcpy(keyName, KEY);
    
    key_t key = ftok(keyName, 'Q'); 
    if(key == -1) {
        printf("can't get key for the file %s\n", keyName);
        return 1;
    }

    int queue = msgget(key, 0);
    if(queue < 0) {
        printf("Can't create queue\n");
        return 2;
    }

    Message msg; 
    int res;
    for(;;) {   
        bzero(msg.buf, 255);
        res = msgrcv(queue, &msg, sizeof(Message), 2L, 0);
        if(res < 0) {
            printf("Error while receiving message\n");
            exit(1);
        }
        printf("new message: %s\n", msg.buf);
    }
    return 0;

}