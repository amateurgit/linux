#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include <dirent.h>

/*
   int scandir(const char *dirp, struct dirent ***namelist,
               int (*filter)(const struct dirent *),							  # 扫描结果只保留 filter 返回 1 的目录名
               int (*compar)(const struct dirent **, const struct dirent **));

 */

static int filter(const struct dirent *entry)
{
    if (!memcmp(&(entry->d_name[0]), "a", 1))   // 选择第一个字母是 a 的文件名
    {
	return 1;
    }

    return 0;
}


int main(void)
{
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, filter, alphasort);
    if (n < 0)
    {
	perror("scandir");
    }
    else 
    {
	while (n--) 
	{
	    printf("%s\n", namelist[n]->d_name);
	    free(namelist[n]);
	}
	free(namelist);
    }

}
