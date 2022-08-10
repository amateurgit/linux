/*
   内嵌汇编语法 :

   	__asm__ __volatile (
	    汇编语句模板
	    输出部分
	    输入部分
	    破坏描述部分
	);

 */

#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned val1 = 1;
    unsigned val2 = 2;
    unsigned val3 = 0;

    printf("val1 = %d, val2 = %d, val3 = %d\n",
	    val1, val2, val3);

    asm volatile (			/* volatile 告诉编译器代码不进行优化 */
	"movl $0, %%eax\n\t"		/* 立即数 0 赋给 寄存器 EAX  */
	"addl %1, %%eax\n\t"		/* 寄存器 ECX 值累加到 EAX 寄存器 */
	"addl %2, %%eax\n\t"		/* 寄存器 EDX 值累加到 EAX 寄存器 */
	"movl %%eax, %0\n\t"		/* EAX 寄存器值赋给内存存储单元 */
	: "=m" (val3)			/* m 表示 memory */	/* %0 */
	: "c" (val1), "d" (val2)	/* c 表示 val1 保存到 ECX 寄存器, d 表示 val2 保存到 EDX 寄存器 */  /* %1, %2 */
	);

    printf("val1:%d + val2:%d = val3:%d\n",
	    val1, val2, val3);

    return 0;
}
