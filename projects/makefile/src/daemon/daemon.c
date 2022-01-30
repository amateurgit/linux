#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/resource.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <syslog.h>
#include <errno.h>

#include "file.h"
#include "error.h"

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE "S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH"

int
already_running(void)
{
	int  fd;
	char buf[16];

	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "can't open %s : %s", LOCKFILE, strerror(errno));
		exit(1);
	}

	if (lockfile(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return (1);
		}
		syslog(LOG_ERR, "can't lock %s : %s", LOCKFILE, strerror(errno));
		exit(1);
	}

	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	return (0);
}

void
daemonize(const char *cmd)
{
	int              i, fd0, fd1, fd2;
	pid_t            pid;
	struct rlimit    rl;
	struct sigaction sa;

	/*
	 * 设置文件模式创建屏蔽字，使守护进程有足够的权限创建文件
	 */
	umask(0);

	/*
	 * 获取进程可以打开的最大文件个数
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_sys("%s:can't get file limit\n", cmd);

	/*
	 * 创建新会话 : a.子进程成为会话首进程; b. 子进程成为一个新进程组的组长进程; c.没有控制终端
	 */
	if ((pid = fork()) < 0)
		err_sys("%s:can't fork\n", cmd);
	else if (pid != 0) /* 父进程 */
		exit(0);

	setsid();


	/*
	 * 忽略 SIGHUP 信号
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_sys("%s:can't ignore SIGHP\n", cmd);

	/*
	 * 不是会话首进程，没有机会被分配到一个控制终端
	 */
	if ((pid = fork()) < 0)
	{
		err_sys("%s:can't fork\n", cmd);
	}
	else if (pid != 0)
		exit(0);

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being unmounted
	 */
	if (chdir("/") < 0)
		err_sys("%s:can't change directory to /", cmd);

	/*
	 * 关闭所有打开的文件描述符
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1 and 2 to /dev/null
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * Initialize the log file
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}
