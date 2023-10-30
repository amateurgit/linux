/*
 * 初始化 Daemon 环境
 * 重载配置文件 (SIGHUP)
 * 进程终止 (SIGTERM)
 * 备注 : 
 *     SIGHUP 终端挂断信号, 默认动作是 终止进程
 *     Daemon 经过两次 fork 失去了终端, 正常情况下不会产生 SIGHUP 信号
 *     因此, 使用 SIGHUP 作为配置重载信号比较可靠
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <pthread.h>

#include "daemonize.h"

#define PIDFILE "/var/run/appd.pid"

sigset_t	mask;

void reread(void)
{
    return;
}

void * thr_fn(void *arg)
{
    int signo;

    for (;;) 
    {
        if (0 != sigwait(&mask, &signo)) 
        {
            syslog(LOG_ERR, "sigwait failed");
            exit(EXIT_FAILURE);
        }

        switch (signo) 
        {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;

            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(EXIT_SUCCESS);

            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }

    return(0);
}

int main(int argc, char *argv[])
{
    char             *app;
    struct sigaction sa; 
    pthread_t		 tid;

    if ( NULL != (app = strrchr(argv[0], '/')) )
    {
        app++;
    }
    else
    {
        app = argv[0];
    }

    /* 初始化 Daemon 环境 */
    daemonize(app);

    /* 单例 */
    if (already_running(PIDFILE))
    {
        syslog(LOG_ERR, "daemon already running");
        exit(EXIT_FAILURE);
    }

    /* 信号处理 */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        exit(EXIT_FAILURE);
    }

    sigfillset(&mask);
    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if (0 != pthread_create(&tid, NULL, thr_fn, 0))
    {
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        /*
         * 其他业务流程
         */
    }

    exit(EXIT_SUCCESS);
}
