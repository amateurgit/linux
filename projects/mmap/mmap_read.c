/*
   gcc -o mmap_read mmap_read.c
 */

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

const char *mmap_file = "/tmp/student";

struct student {
    char name[12];
    int age;
};

int main( int argc, char *argv[] )
{
    int sz = sizeof(struct student);

    /* 创建一个文件 */
    int fd = open( mmap_file, O_RDONLY, 00777 );
    if ( fd < 0 )
    {
        printf("%s %s %d : 创建文件描述符失败\n",
                __FILE__,
                __func__,
                __LINE__);

        exit(EXIT_FAILURE);
    }

    /* 内存映射 */
    struct student *stu = (struct student *)mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
    if ( (void *)-1 == stu )
    {
        printf("%s %s %d : 内存映射失败\n",
                __FILE__,
                __func__,
                __LINE__);

        exit(EXIT_FAILURE);
    }

    /* 读文件 */
    printf("name : %s, age : %d\n", stu->name, stu->age);

    munmap(stu, sz);
    close(fd);

    exit(EXIT_SUCCESS);
}
