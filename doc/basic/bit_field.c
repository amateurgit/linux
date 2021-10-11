#include <stdio.h>

struct bit_st
{
	int a:3;	// 第一个字节的 0~2 位
	int  :0;	// 空域，位 3~7 全 0
	int b:2;	// 第二个字节的 0~1 位
	int c:5;	// 第二个字节的 2~6 位
	int d:2;	// 第三个字节的 0~1 位
	int  :2;	// 无名域名，第三个字节的 2~3 位，不能被使用
	int e:3;	// 第三个字节的 4~6 位
};

int main(int argc, char *argv[])
{
	struct bit_st data, *pdata;
	pdata = &data;

	data.a = 1;
	data.b = 2;	// b 只占 2 位，取值范围 -2~1
	data.c = 3;
	data.d = 2;
	data.e = 4;

	printf("sizeof(int) = %lu\n", sizeof(int));

	printf("a = %d, b = %d, c = %d, d = %d, e = %d\n", data.a, data.b, data.c, data.d, data.e);

	pdata->a = 3;
	pdata->b &= 1;
	pdata->c |= 5;
	pdata->d ^= 1;	// 异或
	pdata->e = 5;

	printf("a = %d, b = %d, c = %d, d = %d, e = %d\n", pdata->a, pdata->b, pdata->c, pdata->d, pdata->e);

	return 0;
}
