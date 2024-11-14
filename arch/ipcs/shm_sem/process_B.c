/*
 * 信号量与共享内存实现一个简单的通信功能
 *     A --> 发送一句话 B --> 读取，发送一句话 A–> 读取，发送一句话
 *
 * B 进程
 *
 */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define SHM_PATH "."
#define SEM_PATH "."
#define PROJ_ID_SHM  10
#define PROJ_ID_SEM  20

#define SHM_SIZE 4096   /* 页大小的整数倍 PAGE_SIZE : 4096 */

int main(int argc, char *argv[])
{
    key_t shm_key, sem_key;
    shm_key = ftok(SHM_PATH, PROJ_ID_SHM);
    sem_key = ftok(SEM_PATH,PROJ_ID_SEM);
    if (-1 == shm_key || -1 == sem_key)
    {
        perror("ftok error");

        return -1;
    }

    /* 获取共享内存和信号量的操作 id */
    int shmid, semid;
    shmid = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0777);  /* 如果不存在, 则创建 */
    semid = semget(shm_key, 2, IPC_CREAT | 0777);     /* int semget(key_t key, int nsems, int semflg); nsems 信号量集合中的信号数量 */
    if (-1 == shmid || -1 == semid)
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

    /* 设置信号量初值 */
    semctl(semid, 0, SETVAL, 1);    /* 0 号信号量设置为 1 */
    semctl(semid, 1, SETVAL, 0);        /* 1 号信号量设置为 0 */

    /* 通信 */
    struct sembuf p_sops_1, v_sops_0;
    p_sops_1.sem_num = 1;   /* 信号编号 */
    p_sops_1.sem_op = -1;   /* P 操作 对信号量减 1, 如果信号量是 0, P 操作阻塞*/
    p_sops_1.sem_flg = 0;
    v_sops_0.sem_num = 0;   /* 信号编号 */
    v_sops_0.sem_op =  1;   /* V 操作 对信号量加 1 */
    v_sops_0.sem_flg = 0;

    while (1)
    {
        /* 对信号 1 进行 p 操作, 阻塞 */
        semop(semid, &p_sops_1, 1); /* int semop(int semid, struct sembuf *sops, size_t nsops); semid : 信号集 id，sops 操作描述,  nsops 操作信号量数组中元素个数 */

        /* 读共享内存 */
        printf("[%d] readbuf = %s", getpid(), shmaddr);

        if (0 == strcmp("EXIT\n", shmaddr))
        {
            break;
        }

        memset(shmaddr, 0, SHM_SIZE);

        /* 信号量 0 加 1, 解除阻塞 */
        semop(semid, &v_sops_0, 1);
    }

    /* 解除映射 */
    shmdt(shmaddr);

    return 0;
}
