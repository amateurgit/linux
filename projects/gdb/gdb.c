#include <stdio.h>   /* 用于printf */
#include <stdlib.h>  /* 用于malloc */
/* 声明函数 sum() 为 staic int 类型 */
static int sum(int value);
/* 用于控制输入输出的结构 */
struct inout {
    int value;
    int result;
};
int main(int argc, char *argv[])
{
    struct inout *io = 	(struct inout *)malloc(sizeof(struct inout)); /* 申请内存 */

    if (NULL == io)
    {
	printf("申请内存失败\n"); /* 打印失败信息 */

	return -1;	/* 返回 -1 */
    }

    if (argc != 2)
    {
	printf("参数输入错误！\n");

	return -1;
    }

    io->value = *argv[1] - '0';
    io->result = sum(io->value);
    printf("你输入的值为:%d, 计算结果为:%d\n", io->value, io->result);

    free(io);

    return 0;
}
/* 累加求和函数 */
static int sum(int value)
{
    int result = 0;
    int i = 0;

    for (i = 0; i < value; i++)	  /* 循环计算累加值 */
	result += i;

    return result; /* 返回结果 */
}
