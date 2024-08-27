#include <stdio.h>
#include <stdlib.h>
#include <mntent.h>

int main(int argc, char *argv[])
{
    struct mntent *mntd = NULL;

    FILE *mounts = setmntent("/proc/mounts", "r");
    if ( NULL == mounts )
    {
        printf("%s %s %d : 文件打开失败\n",
                __FILE__,
                __func__,
                __LINE__);
    }

    while ( (mntd = getmntent(mounts)) )
    {
        printf("#############################\n");
        printf("文件系统名称 : %s\n"
                "挂载路径 : %s\n" 
                "类型 : %s\n" 
                "选项 : %s\n", 
                mntd->mnt_fsname,
                mntd->mnt_dir,
                mntd->mnt_type,
                mntd->mnt_opts
                );
        printf("#############################\n\n\n");
    }

    endmntent( mounts );

    exit(EXIT_SUCCESS);
}
