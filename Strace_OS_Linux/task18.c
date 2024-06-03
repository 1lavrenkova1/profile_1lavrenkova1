#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t t1,t2;
void* thread1(){
    printf("Thread_1 is started\n");
    // здесь код,который должна выполнять нить //   
    time_t start;
    start = time(NULL);
    int n = 1;
    for (int i = 0; i < 2; i++){
        time_t end = time(NULL);
        system("ps axhf > thread1.txt");
        printf("thread1 n = %d, time = %ld\n", n *= 2, end - start);
        sleep(5);
    }
}
void* thread2(){
    printf("Thread_2 is started\n");
    // здесь код, который должна выполнять нить 2//
    time_t start;
    start = time(NULL);
    int n = 1;
    for (int i = 0; i < 2; i++){
        time_t end = time(NULL);
        system("ps axhf > thread2.txt");
        printf("thread2 n = %d, time = %ld\n", n *= 2, end - start);
        sleep(1);
    }
    system("ps axhf > thread2.txt");
}

void main(){
    system("ps axhf > file.txt");
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    system("ps axhf >> file.txt");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    system("ps axhf >> file.txt");
}