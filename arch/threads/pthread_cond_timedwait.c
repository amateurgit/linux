/*

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *start_routine(void *)
{
    struct timespec timewait;
    int ret = 0;

    while ( 1 )
    {
        clock_gettime(CLOCK_MONOTONIC, &timewait); /* CLOCK_MONOTONIC - 系统从 启动到现在 运行的时间 */
        timewait.tv_sec +=  10;

        ret = pthread_cond_timedwait(&cond, &mutex, &timewait);
        if ( 0 == ret )
        {
            printf("条件满足, 执行成功!\n");
        }
        else if ( ETIMEDOUT == ret )
        {
            printf("超时!\n");
        }
        else
        {
            printf("错误代码 : %d\n", ret);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    pthread_condattr_t attr;

    /* 初始化互斥锁 */
    pthread_mutex_init(&mutex, NULL);

    /* 初始化条件 */
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);    /* 条件的时钟属性设置为 CLOCK_MONOTONIC */
    pthread_cond_init(&cond, &attr);
    pthread_condattr_destroy(&attr);

    /* 创建线程 */
    if ( pthread_create(&thread, NULL, start_routine, NULL) != 0 )
    {
        printf("创建线程失败\n");
        exit(EXIT_FAILURE);
    }

    /* 等待子进程结束 */
    pthread_join(thread, NULL);

    exit(EXIT_SUCCESS);
}
