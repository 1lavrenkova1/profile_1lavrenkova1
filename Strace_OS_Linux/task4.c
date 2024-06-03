#include <stdio.h> 
#include <signal.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <fcntl.h>  

void handler(int sig) { 
    if(sig == SIGUSR1) { 
        printf("SIGUSR1 catched, sending SIGUSR2\n"); 
        kill(getpid(), SIGUSR2);
        printf("Sleep SIGUSR1\n");
        sleep(10);
    }
    else if (sig == SIGUSR2){
        printf("SIGUSR2 catched\n"); 
        sleep(5);
        printf("Send SIGUSR1\n");
        kill(getpid(), SIGINT);
    }
    else if (sig == SIGINT){
        printf("Catch SIGINT\n");
        exit(-1);
    }
    else{
        printf("Catched bad signal %d\n",sig); 
        return; 
    }  
} 

int main() { 
    struct sigaction act; 
    act.sa_handler = handler; 
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0; 

    sigaddset(&act.sa_mask,SIGUSR1); 
    sigaddset(&act.sa_mask,SIGUSR2); 
    sigaddset(&act.sa_mask,SIGINT); 

    if(sigaction(SIGUSR1,&act,NULL) < 0) return 1; 
    if(sigaction(SIGUSR2,&act,NULL) < 0) return 1;
    if(sigaction(SIGINT,&act,NULL) < 0) return 1;

    kill(getpid(), SIGUSR1);
    // for(;;) { 
    //     pause(); 
    // } 
    return 0;   
} 