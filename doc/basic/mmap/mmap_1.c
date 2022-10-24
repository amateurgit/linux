#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

typedef struct {
    char name[4];
    int age;
} people;

int main(int argc, char *argv[])
{
    int fd, i;
    people *p_map;
    char temp;

    fd = open(argv[1], O_CREAT|O_RDWR|O_TRUNC, 00777);
    lseek(fd, sizeof(people) * 5 - 1, SEEK_SET);
    write(fd, "", 1);

    /*
       void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
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
    p_map = (people *)mmap(NULL, sizeof(people) * 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if ((void *) -1 == p_map)
    {
	printf("mmap:%s\n", strerror(errno));
	return -1;
    }
    close(fd);

    temp = 'a';
    for (i = 0; i < 10; i++)
    {
	temp += 1;
	(*(p_map + i)).name[1] = '\0';
	memcpy((*(p_map + i)).name, &temp, 1);
	(*(p_map + i)).age = 20 + i;
    }
    printf("initializeover\n");
    sleep(10);
    munmap(p_map, sizeof(people) * 10);
    printf("munmap\n");

    return 0;
}
