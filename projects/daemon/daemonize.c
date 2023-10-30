#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <syslog.h>

#define PIDFILDMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

void daemonize(const char *app)
{
    struct sigaction sa;
    pid_t            pid;
    struct rlimit    rl;
    int i = 0, fd0, fd1, fd2;

    /* 文件权限掩码 0 */
    umask(0);

    /* 进程文件描述符数量 */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        syslog(LOG_ERR, "getrlimit");
        exit(EXIT_FAILURE);
    }

    /* 第一次 fork : 脱离原会话 */
    if ( (pid = fork()) < 0 )
    {
        syslog(LOG_ERR, "fork");
        exit(EXIT_FAILURE);
    }
    else if (0 != pid)
    {
        exit(EXIT_SUCCESS);
    }
    setsid();

    /* 信号处理 */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);   /* sa_mask - 信号处理程序执行期间, 被阻塞的信号 */
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)  /* 忽略终端挂断信号 */
    {
        syslog(LOG_ERR, "sigaction");
        exit(EXIT_FAILURE);
    }

    /* 第二次 fork : 会话首进程退出, 不再分配控制终端 */
    if ((pid = fork()) < 0)
    {
        syslog(LOG_ERR, "fork");
        exit(EXIT_FAILURE);
    }
    else if (pid != 0)
    {
        exit(EXIT_SUCCESS);
    }

    /* 切换工作目录, 避免影响文件系统的挂载操作 */
    if (chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdir");
        exit(EXIT_FAILURE);
    }

    /* 关闭文件描述符 */
    if (rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++)
    {
        close(i);
    }

    /* 标准输入输出描述符附加到 null 设备 */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(app, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", 
                fd0, fd1, fd2);
        exit(EXIT_FAILURE);
    }

    return; 
}

static int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return(fcntl(fd, F_SETLK, &fl));
}

int already_running(const char *pidfile)
{
    int		fd;
    char	buf[16];

    fd = open(pidfile, O_RDWR|O_CREAT, PIDFILDMODE);
    if (fd < 0) 
    {
        syslog(LOG_ERR, "can't open %s: %s", pidfile, strerror(errno));
        exit(1);
    }

    if (lockfile(fd) < 0) 
    {
        if (errno == EACCES || errno == EAGAIN) 
        {
            close(fd);
            return 1;
        }
        syslog(LOG_ERR, "can't lock %s: %s", pidfile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);

    return 0;
} 
