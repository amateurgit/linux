#include <sys/file.h>
#include "error.h"

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type   = type;
	lock.l_start  = offset;
	lock.l_whence = whence;
	lock.l_len    = len;

	return (fcntl(fd, cmd, &lock));
}

pid_t
lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type   = type;
	lock.l_start  = offset;
	lock.l_whence = whence;
	lock.l_len    = len;

	if (fcntl(fd, F_GETLK, &lock) < 0)
		err_sys("fcntl error");

	if (lock.l_type == F_UNLCK)
		return (0);	/* false, region isn't locked by another process */
	return(lock.l_pid);	/* true, return pid of lock owner */
}

int
set_cloexec(int fd)
{
	int val;

	if ((val = fcntl(fd, F_GETFD, 0)) < 0)
		return (-1);

	val |= FD_CLOEXEC;

	return (fcntl(fd, F_SETFD, val));
}
