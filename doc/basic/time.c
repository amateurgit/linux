#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    /* 获取时间戳 */
    time_t timestamp;

    time(&timestamp);
    printf("timestamp = %ld\n", timestamp);


    /* 时间戳转时间 */
    struct tm *ptm;
    char buf[128] = { 0 };

    ptm = gmtime(&timestamp);
    strftime(buf, sizeof(buf), "%4Y-%2m-%2d %2H:%2M:%2S", ptm);
    printf("buf = %s\n", buf);

    /* 时间转时间戳 */
    int year, mon, day, hour, min, sec;
    sscanf(buf, "%4d-%2d-%2d %2d:%2d:%2d", &year, &mon, &day, &hour, &min, &sec);
    printf("year = %d, mon = %d, day = %d, hour = %d, min = %d, sec = %d\n",
	    year, mon, day, hour, min, sec);

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = year - 1900;
    tm.tm_mon  = mon - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min  = min;
    tm.tm_sec  = sec;

    time_t t = mktime(ptm);
    printf("t = %ld\n", t);

    return 0;
}
