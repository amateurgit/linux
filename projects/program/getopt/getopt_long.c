/*
   getopt_long 的返回值和 struct option 结构有关
        struct option {
            const char *name;       // 长选项字符串
            int         has_arg;    // 参数说明 [no_argument|optional_argument|required_argument
            int        *flag;       // flag 不为 NULL, getopt_long 返回 0, *flag = val; flag 为 NULL, getopt_long 返回 val 
            int         val;
        };
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static int name, age;

struct option long_options[] = {
    {"name", no_argument, &name, 1},
    {"age", optional_argument, &age, 1},
    {"sex", required_argument, NULL, 's'},
    {0, 0, 0, 0},
};

int main(int argc, char *argv[])
{
    const char *optstring = "s:";
    int ch;
    int option_index = 0;

    while (1)
    {
        ch = getopt_long(argc, argv, optstring, long_options, &option_index);
        if (-1 == ch)
        {
            break;
        }

        printf("option_index = %d\n", option_index);

        switch (ch)
        {
            case 0:
                printf("name = %d\n", name);
                printf("age = %d\n", age);
                break;
            case 's':
                printf("optarg = %s\n", optarg);
                break;
            default:  /* ? */
                printf("无效选项 或 缺少参数\n");
                break;
        }
    }

    exit(EXIT_SUCCESS);
}
