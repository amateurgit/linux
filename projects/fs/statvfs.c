#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/statvfs.h>

#define GB (1024*1024*1024)

int main(int argc, char *argv[])
{
    struct statvfs vfs;

    memset(&vfs, 0, sizeof(struct statvfs));
    if ( statvfs("/", &vfs) < 0 )
    {
        printf("%s %s %d : 获取文件系统信息失败\n",
                __FILE__,
                __func__,
                __LINE__);

        return -1;
    }

    fsblkcnt_t block_size = vfs.f_bsize;
    printf("block 大小 : %ld\n", block_size);

    fsblkcnt_t total_size = vfs.f_blocks * block_size;
    printf("总容量 : %0.2lf (GB)\n", (double)total_size / (GB));

    /*
       f_bfree 和 f_bavail 区别 :
           f_bavail 表示普通用户可用空间, f_bfree 包含了系统为 root 账户分配的保留空间
     */

    fsblkcnt_t free_size = vfs.f_bfree * block_size;
    printf("硬盘未分配 : %0.2lf (GB)\n", (double)free_size / (GB));

    fsblkcnt_t avail_size = vfs.f_bavail * block_size;
    printf("普通用户可用容量 : %0.2lf (GB)\n", (double)avail_size / (GB));

    fsblkcnt_t used_size = (vfs.f_blocks - vfs.f_bfree) * block_size;
    printf("已使用容量 : %0.2lf (GB)\n", (double)used_size / (GB));

    exit(EXIT_SUCCESS);
}
