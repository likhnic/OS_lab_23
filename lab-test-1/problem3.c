#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define UNPROCESSED 0
#define PROCESSED 1

int **M;
int *A, *B;
int count = 0, in = 0, out = 0;
int m, n;

pthread_mutex_t *rowLock;
pthread_mutex_t lockAB = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condABFull = PTHREAD_COND_INITIALIZER, condABEmpty = PTHREAD_COND_INITIALIZER;


void *chaosRunner(void *arg){
    
    printf("I am chaos\n");
    for(int i=0;i<30;++i){

        int row = (rand()%m), col = rand()%n;
        pthread_mutex_lock(&rowLock[row]);
        int updated = (rand()%1000)+1;
        M[row][col] = updated;
        printf("Chaos: updated element at %d x %d with value %d\n", row, col, updated);
        pthread_mutex_unlock(&rowLock[row]);

        pthread_mutex_lock(&lockAB);

        while(count==1000){
            pthread_cond_wait(&condABFull, &lockAB);
        }
        A[in] = row;
        B[in] = UNPROCESSED;

        in++;
        count++;

        pthread_cond_broadcast(&condABEmpty);

        pthread_mutex_unlock(&lockAB);

        sleep(2);
    }

    printf("CHAOS ENDS\n");
    return NULL;
}

void *orderRunner(void *arg){

    printf("I am order\n");
    for(int i=0;i<30;++i){

        pthread_mutex_lock(&lockAB);
        while(count == 0){
            pthread_cond_wait(&condABEmpty, &lockAB);
        }

        int row = A[out];
        B[out] = PROCESSED;

        out++;
        count--;
        printf("Order: detected updated element at row %d\n", row);

        pthread_cond_broadcast(&condABFull);
        pthread_mutex_unlock(&lockAB);

        pthread_mutex_lock(&rowLock[row]);

        int old[n], new[1001];
        
        for(int j=0;j<1001;++j)new[j]=0;

        for(int j=0;j<n;++j){
            old[j] = M[row][j];
            new[M[row][j]]++;
        }

        int curr=0;
        for(int j=0;j<1001;++j){
            for(int k=0;k<new[j];++k){
                M[row][curr++] = j;
            }
        }

        printf("Order row %d is sorted now\n", row);
        printf("older row %d: ",row);

        for(int j=0;j<n;++j)printf("%d ", old[j]);
        printf("\n");

        printf("new row %d: ",row);

        for(int j=0;j<n;++j)printf("%d ", M[row][j]);
        printf("\n");

        pthread_mutex_unlock(&rowLock[row]);
        
    }

    return NULL;
}

int main(){

    srand(time(0));
    pthread_t orderThread[3], chaosThread;

    
    scanf("%d%d", &m, &n);

    M = (int **)malloc(sizeof(int *)*m);

    for(int i=0;i<m;++i){
        M[i] = (int*)malloc(sizeof(int)*n);
        for(int j=0;j<n;++j){
            M[i][j] = (rand()%1000)+1;
        }
    }

    printf("Random matrix M is created\n");

    
    A = (int*)malloc(sizeof(int)*1000);
    B = (int*)malloc(sizeof(int)*1000);


    printf("Shared arrays A and B are created\n");

    for(int i=0;i<m;++i){
        for(int j=0;j<n;++j){
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }

    rowLock = (pthread_mutex_t *)malloc(m*sizeof(pthread_mutex_t));

    for(int i=0;i<m;++i){
        pthread_mutex_init(&rowLock[i], NULL);
    }

    pthread_create(&chaosThread, NULL, chaosRunner, NULL);
    for(int i=0;i<3;++i){
        pthread_create(&orderThread[i], NULL, orderRunner, &i);
    }

    for(int i=0;i<3;++i){
        pthread_join(orderThread[i], NULL);
    }
    pthread_join(chaosThread, NULL);

    for(int i=0;i<m;++i){
        pthread_mutex_destroy(&rowLock[i]);
    }

    pthread_mutex_destroy(&lockAB);

}