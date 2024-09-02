#include <stdio.h>
#include <stdlib.h>
#include <mntent.h>

int main(int argc, char *argv[])
{
    FILE *mounts = setmntent("/proc/mounts", "r");
    if ( NULL == mounts )
    {
        printf("%s %s %d : 文件打开失败\n",
                __FILE__,
                __func__,
                __LINE__);
    }

#if 0
    struct mntent *mntd = NULL;

    while ( (mntd = getmntent(mounts)) )
    {
        printf("#############################\n");
        printf("文件系统 : %s\n"
                "挂载点 : %s\n" 
                "文件系统类型 : %s\n" 
                "选项 : %s\n", 
                mntd->mnt_fsname,
                mntd->mnt_dir,
                mntd->mnt_type,
                mntd->mnt_opts
              );
        printf("#############################\n\n\n");
    }
#else
    struct mntent mntd;
    char buff[1024];

    while ( getmntent_r(mounts, &mntd, buff, sizeof(buff)))
    {
        printf("#############################\n");
        printf("文件系统 : %s\n"
                "挂载点 : %s\n" 
                "文件系统类型 : %s\n" 
                "选项 : %s\n", 
                mntd.mnt_fsname,
                mntd.mnt_dir,
                mntd.mnt_type,
                mntd.mnt_opts
              );

        printf("buff = %s\n", buff);
        printf("#############################\n\n\n");
    }
#endif

    endmntent( mounts );

    exit(EXIT_SUCCESS);
}
