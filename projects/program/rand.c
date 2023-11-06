#include <stdio.h>
#include <stdlib.h>

#include <time.h>

int myrandom(int min, int max)
{
    time_t timestamp = time(NULL);

    srand((unsigned int)timestamp);

    return (min + rand() % (max - min + 1));
}

int main(int argc, char *argv[])
{
    int i = 0;
    printf("%d\n", myrandom(95,100));

    exit(EXIT_SUCCESS);
}
