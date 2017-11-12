/*
  1. 一个buffer
  2. buffer的锁
  3. 通知机制,producer写好之后通知consumer 
  4. 一个生产者线程
  5.一个消费者线程
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>

//buffer数据类型 可扩展
typedef struct
{
    int num;
    struct tm *ploc;
}BUFFER;

#define MAX_NUM 1000

BUFFER buf;
//lock
pthread_mutex_t pc_mutex;
//通知
pthread_cond_t pc_condp, pc_condc;


/*
  生产者
 */

void * producer(void * nul)
{
    int i;
    for (i = 1; i < MAX_NUM; ++i)
    {
        pthread_mutex_lock(&pc_mutex);

        //等待条件变量
        while(buf.num != 0)
        {
            pthread_cond_wait(&pc_condp, &pc_mutex);
        }
        //生产
        buf.num = i;
        time_t cur = time(NULL);
        buf.ploc = localtime(&cur) ;
        printf("producer produces %d ", buf.num );
        printf("at %s\n", ctime(&cur));
        //通知
        pthread_cond_signal(&pc_condc);
        pthread_mutex_unlock(&pc_mutex);
    }
    pthread_exit(NULL);

}

/*
  消费者
 */

void * consumer(void * nul)
{
    int i;
    for (i = 1; i < MAX_NUM; ++i)
    {
        pthread_mutex_lock(&pc_mutex);

        //等待条件变量
        while(buf.num == 0) {
            pthread_cond_wait(&pc_condc, &pc_mutex);
        }
        //条件达到
        time_t cur = time(NULL);
        buf.ploc = localtime(&cur) ;
        printf("consumer consumes %d ", buf.num);
        printf("at %s\n", ctime(&cur));
        buf.num = 0;
        //通知consumer
        pthread_cond_signal(&pc_condp);
        pthread_mutex_unlock(&pc_mutex);
    }

    pthread_exit(NULL);
}



int main(int argc, char const *argv[])
{
    pthread_t thread[2];
    pthread_attr_t attr;

    buf.num = 0;

    //锁和条件变量
    pthread_mutex_init(&pc_mutex, NULL);
    pthread_cond_init(&pc_condp, NULL);
    pthread_cond_init(&pc_condc, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //producer
    pthread_create(&thread[0], &attr, producer, NULL);
    //consumer
    pthread_create(&thread[1], &attr, consumer, NULL);

    //连接线程
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    //清理资源
    pthread_mutex_destroy(&pc_mutex);
    pthread_cond_destroy(&pc_condc);
    pthread_cond_destroy(&pc_condp);
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    return 0;
}
