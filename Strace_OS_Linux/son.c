#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){
    int pid,ppid;
    pid=getpid();
    ppid=getppid();
    printf("\n\nSON PARAMS:  pid=%i  ppid=%i\n\n",pid,ppid);
    sleep(5);
    //exit(1);статус завершения 256
    return 0;// статус завершения 0  
}