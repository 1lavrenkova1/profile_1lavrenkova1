#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int pid, ppid, status;
    pid=getpid();
    ppid=getppid();
    printf("\n\nFATHER PARAM: pid=%i  ppid=%i\n", pid,ppid);
    if(fork()==0) execl("son","son", NULL);
    system("ps -xf > test5.txt");
    wait(&status);
    printf("\n\nChild proccess is finished with status %d\n\n", status);
    return 0;
}