/*
   宏, # 与 ## 的用法
   1. #  形参转字符串
   2. ## 粘合剂
 */

#include <stdio.h>
#include <sys/types.h>

#define STR(s) #s            /* # 转换成字符串 */
#define def_u32_array(__name, __size) u_int32_t array_##__name[__size];  /* 粘合剂 */

int main(int argc, char argv[])
{
    printf("str = %s\n", STR(hello world!));

    def_u32_array(sample_buffer, 64);	// 宏展开 array_sample_buffer[64];
    array_sample_buffer[0] = 100;
    printf("array_sample_buffer[0] = %d\n", array_sample_buffer[0]);

    return 0;
}
