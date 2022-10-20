/*
   宏, # 与 ## 的用法
   1. #  形参转字符串
   2. ## 粘合剂
 */

#if 0

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
#endif


#if 0

#include <stdio.h>
#include <string.h>

/* 类型 */
typedef struct student {
    int age;
    char name[128];
}stu;

/* 接口模板 */
#define IMPL_CFG_TABLE_TYPE_GET(typename, type)	\
static inline type *nCfg_find_##typename##_by_name(const char *name)	\
{	\
    printf("name = %s\n", name);	\
    \
    return NULL;	\
}	\
\
static inline type *nCfg_find_##typename##_by_id(int id)	\
{	\
    printf("id = %d\n", id);	\
    \
    return NULL;	\
}	\

/* 实例化 */
IMPL_CFG_TABLE_TYPE_GET(test ,stu)

/* 引用 */
int main(int argc, char *argv[])
{
    struct student s;
    snprintf(s.name, sizeof(s.name), "sjwang");
    s.age = 35;

    nCfg_find_test_by_name(s.name);
    nCfg_find_test_by_id(s.age);

    return 0;
}

#endif
