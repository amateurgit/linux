/*
   遍历指定下的文件
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    const char *name = "/";

    DIR *dirp = NULL;
    struct dirent *dir_ent = NULL;

    dirp = opendir(name);
    if (NULL == dirp)
    {
	perror("opendir");

	exit(-1);
    }

    errno = 0; // distinguish end of stream from an error
    while (NULL != (dir_ent = readdir(dirp)))
    {
	if (0 != errno)
	{
	    perror("readdir");
	    closedir(dirp);

	    exit(-1);
	}

	printf("%s\n", dir_ent->d_name);
    }

    closedir(dirp);

    exit(0);
}
