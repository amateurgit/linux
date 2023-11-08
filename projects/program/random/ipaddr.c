#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <stdint.h>

static int myrandom(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

static void rand_ipaddr(char ipaddr[], int len)
{
    uint8_t a = myrandom(0, 255);
    uint8_t b = myrandom(0, 255);
    uint8_t c = myrandom(0, 255);
    uint8_t d = myrandom(0, 255);

    snprintf(ipaddr, len, "%u.%u.%u.%u", a, b, c, d);

    return;
}

int main(int argc, char *argv[])
{
    char ipaddr[16];
    int i = 0;

    srand(time(NULL));

    for (i = 0; i < 16; i++)
    {
        rand_ipaddr(ipaddr, sizeof(ipaddr));
        printf("ipaddr = %s\n", ipaddr);
    }

    exit(EXIT_FAILURE);
}
