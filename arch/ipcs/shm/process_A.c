/*
 * 进程 A 和进程 B 使用共享内存进行通信
 * 进程 A 往共享内存写入自己的PID, 进程B读取共享内存数据，给目标进程发送信号
 *
 * A 进程
 *
 */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_PATH "."
#define PROJ_ID  5
#define SHM_SIZE 4096 * 2	/* 页大小的整数倍 PAGE_SIZE : 4096 */

int main(int argc, char *argv[])
{
    key_t key = ftok(SHM_PATH, PROJ_ID);
    if (-1 == key)
    {
	perror("ftok error");

	return -1;
    }

    /* 获取或创建 */
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0777);  /* 如果不存在, 则创建 */
    if (-1 == shmid)
    {
	perror("shmget error\n");

	return -1;
    }

    /* 映射共享内存到进程空间 */
    char *shmaddr = shmat(shmid, NULL, 0);
    if ((void *)-1 == shmaddr)
    {
	perror("shmat error\n");

	return -1;
    }

    /* 写共享内存 */
    printf("pid = %d\n", getpid());
    *(pid_t *)shmaddr = getpid();

    /* 等待信号 */
    pause();

    /* 解除映射 */
    shmdt(shmaddr);

    return 0;
}

























