#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 8				/* 线程数量 */
#define NUMNUM 8000000L			/* 需要排序的成员的个数 */
#define TNUM (NUMNUM/NTHR)              /* 每个线程排序的任务量 */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;			/* 屏障变量 */

int complong(const void *arg1, const void *arg2)
{
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if (l1 == l2)
	return 0;
    else if (l1 < l2)
	return -1;
    else
	return 1;
}

/*
 * 工作线程 : 每个线程完成部分元素排序任务
 */
void *thr_fn(void *arg)
{
    long idx = (long)arg;

    /*
       排序算法
           void qsort(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *));
	   base  : 参加排序的数组的首元素地址
           nmemb : 元素个数
           size  : 每个元素占用的内存大小
           compar: 排序方法
     */
    qsort(&nums[idx], TNUM, sizeof(long), complong);

    /********** 2. 等待其他线程 ***********/
    pthread_barrier_wait(&b);

    return ((void *)0);
}

/*
 *  在各个分段中, 遍历查找最小元素的下标, 依次写入结果数组, 实现合并
 */
void merge()
{
    long idx[NTHR];
    long i, minidx, sidx, num;

    for (i = 0; i < NTHR; i++)
	idx[i] = i * TNUM;      /* idx 分段首元素下表 */

    for (sidx = 0; sidx < NUMNUM; sidx++)  /* sidx 遍历全部元素 */
    {
	num = LONG_MAX;
	for (i = 0; i < NTHR; i++)  /* 遍历各个分段元素 */
	{
	    if ((idx[i] < (i + 1) * TNUM) && (nums[idx[i]] < num))
	    {
		num = nums[idx[i]];
		minidx = i;		/* 查找最小元素所在分段的下标 */
	    }
	}
	snums[sidx] = nums[idx[minidx]];
	idx[minidx]++;
    }
}

int main(int argc, char *argv[])
{
    unsigned long i;
    struct timeval start, end;
    long long startusec, endusec;
    double elapsed;
    int err;
    pthread_t tid;

    /*
     * 生成伪随机数
     */
    srandom(1);	  /* 为 random 设置 seed */
    for (i = 0; i < NUMNUM; i++)
	nums[i] = random();


    gettimeofday(&start, NULL);	/* 时间戳 */

    /********** 1. 初始化屏障变量 ***********/
    pthread_barrier_init(&b, NULL, NTHR + 1);	

    for (i = 0; i < NTHR; i++)
    {
	err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
	if (err != 0)
	{
	    printf("cann't create thread\n");
	    exit(1);
	}
    }

    /********** 2. 等待其他线程 ***********/
    pthread_barrier_wait(&b);

    merge();
    gettimeofday(&end, NULL);

    /* 打印排序列表 */
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec   = end.tv_sec * 1000000 + end.tv_usec;
    elapsed = (double)(endusec - startusec) / 1000000.0;
    printf("sort took %.4f seconds\n", elapsed);
    for (i = 0; i < NUMNUM; i++)
	printf("%ld\n", snums[i]);

    exit(0);
}





















