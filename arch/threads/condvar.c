#include <pthread.h>

struct msg {
    struct msg *m_next;
    /* ... more stuff here ... */
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg(void)
{
    struct msg *mp;

    for (;;) {
	pthread_mutex_lock(&qlock);
	while (workq == NULL)
	    pthread_cond_wait(&qready, &qlock);  /* 队列为空阻塞, 自动释放互斥锁; 接收到条件信号, 自动获取互斥锁 */
	mp = workq;
	workq = mp->m_next;
	pthread_mutex_unlock(&qlock);

	/* now process the message mp */
    }
}

void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);     /* 消息入队列, 发送条件信号 */
}
