// gcc -o mmap_write mmap_write.c

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
    int fd = open( mmap_file, O_CREAT | O_RDWR | O_TRUNC, 00777 );
    if ( fd < 0 )
    {
        printf("%s %s %d : 创建文件描述符失败\n",
                __FILE__,
                __func__,
                __LINE__);

        exit(EXIT_FAILURE);
    }

    ftruncate(fd, sz);

    /* 内存映射 */
    struct student *stu = (struct student *)mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ( (void *)-1 == stu )
    {
        printf("%s %s %d : 内存映射失败\n",
                __FILE__,
                __func__,
                __LINE__);

        exit(EXIT_FAILURE);
    }

    /* 写文件 */
    snprintf(stu->name, sizeof(stu->name), "sjwang");
    stu->age = 38;

    munmap(stu, sz);
    close(fd);

    exit(EXIT_SUCCESS);
}

/*
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    addr   : 映射区内存地址
    length : 映射区长度
    prot   : 访问权限
    PROT_EXEC  //页内容可以被执行
    PROT_READ  //页内容可以被读取
    PROT_WRITE //页可以被写入
    PROT_NONE  //页不可访问
    flags  : 映射对象的类型
    MAP_SHARED  进程共享, 直到msync()或者munmap()被调用，文件实际上才会更新
    fd     : 文件描述符
    offset : 文件位置
*/
