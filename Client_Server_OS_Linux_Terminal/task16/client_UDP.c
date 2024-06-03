#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/time.h>

#define DEF_PORT 8888 
#define DEF_IP "127.0.0.1" 

int main( int argc, char** argv) {      
    char* addr; 
    int port;      
    char* readbuf;  
    if(argc < 3) { 
        printf("Using default port %d\n",DEF_PORT); 
        port = DEF_PORT; 
    } 
    else port = atoi(argv[2]);   

    if(argc < 2) { 
        printf("Using default addr %s\n",DEF_IP);           
        addr = DEF_IP;      
    } 
    else addr = argv[1]; // создаем сокет 

    struct sockaddr_in peer;      
    peer.sin_family = AF_INET;      
    peer.sin_port = htons( port );      
    peer.sin_addr.s_addr = inet_addr( addr );      
    int sock = socket( AF_INET, SOCK_DGRAM, 0 );      
    if ( sock < 0 ){           
        perror( "Can't create socket\n" );           
        exit( 1 );      
    } 

    char buf[100];  
    int first_msg = 1;
    int peer_addr_size = sizeof(peer);    
    for(;;) { 
        printf("Input request (empty to exit)\n"); 
        bzero(buf,100);         
        fgets(buf, 100, stdin); 
        buf[strlen(buf)-1] = '\0'; 

        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        int res = sendto( sock, buf, strlen(buf), 0,( struct sockaddr * )&peer, sizeof( peer )); 
        if(strlen(buf) == 0) { 
            printf("Bye-bye\n"); 
            return 0; 
        } 
        if ( res <= 0 ) { 
            perror( "Error while sending:" ); 
            exit( 1 ); 
        } 
        bzero(buf,100); 
        res = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr *)&peer, &peer_addr_size);

        gettimeofday(&end_time, NULL);
        long microseconds = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

        if ( res <= 0 ) { 
            perror( "Error while receiving:" ); 
            exit(1); 
        } 
        printf("Server's response: %s\n",buf); 
        printf("TIME: %ld mcs\n",microseconds); 
    } 
    return 0; 
} 