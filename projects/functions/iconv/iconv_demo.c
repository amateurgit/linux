#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iconv.h>

#define F_GB2312 "./GB2312.csv"
#define MAXLINE 1024

/*
 * 从 GB2312 格式编码的文件中读取一行，转为 UTF-8 格式， 打印出来
 */
/*
 * 命令行转换方式 : iconv -f "GB2312" -t "UTF-8" GB2312.csv > UTF8.csv 
 */

int main(int argc, char *argv)
{
    /* 声明 */
    FILE *fp;

    iconv_t cd;
    char inbuf[MAXLINE];
    char *in = inbuf;
    size_t inbytesleft = sizeof(inbuf);
    char outbuf[MAXLINE];
    char *out = outbuf;
    size_t outbytesleft = sizeof(outbuf);
    size_t size;



    /* 从文件读取一行字符串 */
    fp = fopen(F_GB2312, "r");
    if (NULL == fp)
    {
	printf("Failed to open file:%s", F_GB2312);

	return -1;
    }

    while (NULL != (fgets(inbuf, MAXLINE, fp)))
    {
	printf("inbuf = %s\n", inbuf);

	break;
    }

    fclose(fp);

    /* 编码转换 */
    cd = iconv_open("UTF-8", "GB2312");
    if (cd < 0)
    {
	printf("Failed to exec iconv_open");

	return -1;
    }

    size = iconv(cd, &in, &inbytesleft, &out, &outbytesleft);
    if (size < 0)
    {
	printf("Failed to exec iconv");
    }

    iconv_close(cd);


    /* 打印 */
    printf("outbuf = %s\n", outbuf);

    exit(0);
}
