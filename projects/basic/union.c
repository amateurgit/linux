/* 相同的内存空间 存储 不同类型的数据 */
#include <stdio.h>

union Data
{
    int i;
    char str[16];
};

int main(int argc, char *argv[])
{
    /* 1. union 占用内存大小 */
    printf("sz = %ld\n", sizeof(union Data));

    /* 2. union 成员的使用  */
    union Data data = { 0 };
    data.i = 10;
    printf("i = %d\n", data.i);

    snprintf(data.str, sizeof(data.str), "Hello");
    printf("str = %s\n", data.str);

    printf("i = %d\n", data.i); /* 成员 i 被 str 覆盖, 输出不再是 10 */

    return 0;
}
