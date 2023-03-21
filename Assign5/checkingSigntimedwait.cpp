#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

pthread_t p1, p2;

void* t1(void *arg){

    // send SIGUSR1 to thread running thread 2
    printf("Sending SIGUSR1\n");
    sleep(10);
    pthread_kill(p2, SIGUSR1);
    printf("Sent SIGUSR1\n");

    return NULL;


}

void *t2(void *arg){

    // wait for SIGUSR1

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    int sig;
    struct timespec timeout;
    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;
    int v = sigtimedwait(&mask, NULL, &timeout);
    if(errno == EAGAIN){
        printf("Timeout %d\n", v);
    }
    else if(v==SIGUSR1){
        printf("Received SIGUSR1\n");
    }

    return NULL;

}

int main (int argc, char *argv[])
{

    // signal(SIGUSR1, handle_sig);// 
    // sigset_t mask;
    // sigset_t orig_mask;
    // struct timespec timeout;

 
    // sigemptyset (&mask);
    // sigaddset (&mask, SIGINT);
 
    // if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
    //     perror ("sigprocmask");
    //     return 1;
    // }
 
    // timeout.tv_sec = 5;
    // timeout.tv_nsec = 0;
 
    
    // int v =sigtimedwait(&mask, NULL, &timeout);
    // if (errno == EAGAIN) {
    //     printf ("Timeout\n");
    //     return -1;
    //    }
    // if(v== SIGINT){
    //     printf("SIGINT\n");
    //     return 1;
    //     }

    
    pthread_create(&p1, NULL, &t1, NULL);
    pthread_create(&p2, NULL, &t2, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

 
    return 0;
}