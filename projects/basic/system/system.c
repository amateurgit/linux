/*
   system

       阶段 1 : 
           创建子进程, 失败则返回 -1

       阶段 2 :
           调用 /bin/sh, 执行脚本
           注 : 只要能调用到 /bin/sh, 且脚本执行期间未被信号中断, 认为正常结束

       阶段 3 :
           脚本正常结束, 返回值写到 status

 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int status = system("./ls.sh");

    if (-1 == status)                           /* 阶段 1 */
    {
        printf("system 创建子进程失败\n");  
    }
    else
    {
        if (WIFEXITED(status))                  /* 阶段 2 */
        {
            printf("脚本执行正常结束, 退出码 : %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("system 调用 /bin/sh 失败, 或脚本执行期间被其他信号中断, 异常退出代码 : %d\n", WEXITSTATUS(status));
        }
    }

    exit(EXIT_SUCCESS);
}
