#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t tt; 
    struct tm *t; 

    //    tt = time(NULL);
    asm volatile (
	"mov $0 ,%%ebx\n\t"	/* 系统调用传递第一个参数使用 ebx 寄存器为 0 */
	"mov $0xd, %%eax\n\t"   /* 使用 eax 传递系统调用号 13, 系统调用号列表 /usr/include/asm/unistd_32.h */
	"int $0x80\n\t"		/* 触发系统调用 */
	"mov %%eax, %0\n\t"

	: "=m" (tt)
    );

    t = localtime(&tt);
    printf("time:%d-%d-%d %d:%d:%d\n",
            t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return 0;
}
