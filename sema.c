#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAX 10 //队列长度
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t full;     //填充的个数
sem_t empty;    //空槽的个数
 
 
int total = 0; //
int amount = 0;
void* produce(void* arg)
{
    int i;

    for ( i = 0; i < MAX*2; i++)
    {
        //printf("producer is preparing data/n ");
        sem_wait(&empty);//若空槽个数低于0阻塞
    if(total == 50){
      return (void*)1;
    } 
        //printf("emptyis %d\n", &empty);
        pthread_mutex_lock(&mutex);
        amount = amount+1;
        //top = (top+1) % MAX;
        printf("Stack amount is %d\n", amount);
     
        pthread_mutex_unlock(&mutex);
        
        sem_post(&full);
        sleep(0.05);
    }

    return (void*)1;
}
 
void* consume(void* arg)
{
    int i;
    for (;total < 50;)
    {
        //printf("consumer is preparing data ");
        sem_wait(&full);//若填充个数低于0阻塞
     
        pthread_mutex_lock(&mutex);
        amount = amount-1; 
        total = total + 1;
        //bottom = (bottom+1) % MAX;
        printf("Stack amount is %d   ", amount);
        printf("Already Consumed %d items\n", total);
        pthread_mutex_unlock(&mutex);
         
        sem_post(&empty);
sleep(0.01);
    }
 
    return (void*)2;
}
 
int main(int argc, char *argv[])
{
    pthread_t thid1;
    pthread_t thid2;
    pthread_t thid3;
    pthread_t thid4;
    pthread_t thid5;
 
    int ret1;
    int ret2;
    int ret3;
    int ret4;
    int ret5;
 
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, MAX);
 
    pthread_create(&thid1, NULL, produce, NULL);
    pthread_create(&thid2, NULL, consume, NULL);
    pthread_create(&thid3, NULL, produce, NULL);
    pthread_create(&thid4, NULL, consume, NULL);
    pthread_create(&thid5, NULL, produce, NULL);

    pthread_join(thid1, (void**)&ret1);
    pthread_join(thid2, (void**)&ret2);
    pthread_join(thid3, (void**)&ret3);
    pthread_join(thid4, (void**)&ret4);
    pthread_join(thid4, (void**)&ret5);
 
    return 0;
}
