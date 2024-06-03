#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define DEF_PORT 8888
#define DEF_IP "127.0.0.1"

int main(int argc, char** argv){
    int port = 0;
    if (argc < 2){
        printf("Using DEF_PORT %d\n", DEF_PORT);
        port = DEF_PORT;
    }
    else port = atoi(argv[1]);

    struct sockaddr_in listenerInfo;
    listenerInfo.sin_family = AF_INET;
    listenerInfo.sin_port = htons(port);
    listenerInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    int listener = socket(AF_INET, SOCK_DGRAM, 0);

    if (listener < 0){
        perror("Can't create socket to listen: ");
        exit(1);
    }

    int res = bind(listener, (struct sockaddr_in*)&listenerInfo, sizeof(listenerInfo));
    if (res < 0){
        perror("Can't bind socket\n");
        exit(1);
    }

    char buf[100];
    struct sockaddr_in client;
    int client_size = sizeof(client);

    for (;;){
        bzero(buf, 100);
        res = recvfrom(listener, buf, sizeof(buf)-1, 0, (struct sockaddr_in *)&client, &client_size);
        if (res <= 0){
            perror("Can't recv data from client\n");
            exit(1);
        }

        printf("Some client send: %s\n", buf);
        res = sendto(listener, buf, strlen(buf), 0, (struct sockaddr_in *)&client, client_size);
        if (res <= 0){
            perror("Send faild\n");
            exit(1);
        }
    }

    return 0;
}