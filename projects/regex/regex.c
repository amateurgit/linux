/* 匹配模式, 并打印匹配到的字符串 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

static const char *const str = 
"1) John Driverhacker;\n2) John Doe;\n3) John Foo;\n";
static const char *const re = "John.*o";

int main(int argc, char *argv[])
{
    static const char *s = str;
    regex_t regex;
    regmatch_t pmatch[1];
    regoff_t off, len;

    /*
       1. 编译模式字符串
          REG_NEWLINE  : 提供对 '^ 匹配行首; $ 匹配行末' 的支持
	  REG_EXTENDED : 功能扩展, 设定此项才可以支持丰富的正则符号
          REG_ICASE    : 忽略大小写
          REG_NOSUB    : 不要匹配结果
     */
    if (regcomp(&regex, re, REG_NEWLINE | REG_EXTENDED))
    {
	exit(EXIT_FAILURE);
    }

    printf("String = \"%s\"\n", str);
    printf("Matches:\n");

    for ( int i = 0; ; i++ )
    {
	/* 
	   2. 匹配
	     regex  模式
	     s      目标字符串
	     pmatch 结果
	 */
	if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
	{
	    break;
	}

	/*
	   rm_so 字符串起始位置
	   rm_eo 字符串结束位置
	 */
	off = pmatch[0].rm_so + (s - str);
	len = pmatch[0].rm_eo - pmatch[0].rm_so;

	printf("#%d:\n", i);
	printf("offset = %jd; length = %jd\n", (intmax_t) off,
		(intmax_t) len);
	/*
	   %.*s : * 对应一个整数, . 最大, .* 字符串宽度大于这个值, 按这个值输出, 小于这个值, 按实际宽度输出。
	 */
	printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so);

	s += pmatch[0].rm_eo;
    }

    /* 3 释放内存 */

    exit(EXIT_SUCCESS);
}
