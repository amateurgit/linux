/*
   1. 选项字符串 optstring 规则
       不带参数的选项可以写在一起 
       :  - 必选参数, 选项和参数间空格可选
       :: - 可选参数, 参数和选项间不能有空格
   2. 错误处理的两种方式
       opterr
       optstring 首字符使用冒号
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define COLON

int main(int argc, char *argv[])
{
#ifdef COLON
    const char *optstring = ":abc:d::";
#else
    const char *optstring = "abc:d::";
#endif

    int ch;

#ifndef COLON
    opterr = 0;  /* 禁用 getopt 输出错误信息 */
#endif

    while( (ch = getopt(argc, argv, optstring)) != -1 )
    {
        switch(ch)
        {
            case 'a':
                printf("%c\n", ch);
                break;
            case 'b':
                printf("%c\n", ch);
                break;
            case 'c':
                printf("-%c %s\n", ch, optarg);
                break;
            case 'd':
                printf("-%c %s\n", ch, optarg);
                break;
#ifdef COLON /* 选项字符串 optstring 首字符是 冒号, getopt 不再输出错误信息, 出错时 返回 '?' 或 ':' */
            case '?':
                printf("无效选项字符\n"); 
                break;
            case ':':
                printf("缺少选项参数\n");
                break;
#else
            case '?':
                printf("无效选项字符 或 缺少参数\n");      /* 选项错误 或 缺少参数, getopt 返回错误信息并返回 ? */
                break;
#endif
        }
    }

    exit(EXIT_FAILURE);
}
