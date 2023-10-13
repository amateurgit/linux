#include <stdio.h>

#define FONTSIZE 10

void print_hello(void);

void print_hello(void)
{
    int i, j;

    char data_H[FONTSIZE][FONTSIZE] =
    {
	"          ",
	" *      * ",
	" *      * ",
	" *      * ",
	" * *  * * ",
	" *      * ",
	" *      * ",
	" *      * ",
	" *      * ",
	"          "
    };

    char data_E[FONTSIZE][FONTSIZE] =
    {
	"          ",
	"  * * * * ",
	"  *       ",
	"  *       ",
	"  * * *   ",
	"  *       ",
	"  *       ",
	"  *       ",
	"  * * * * ",
	"          "
    };

    char data_L[FONTSIZE][FONTSIZE] =
    {
	"          ",
	" *        ",
	" *        ",
	" *        ",
	" *        ",
	" *        ",
	" *        ",
	" *        ",
	" * * * *  ",
	"          "
    };

    char data_O[FONTSIZE][FONTSIZE] =
    {
	"          ",
	"   ****   ",
	"  *    *  ",
	" *      * ",
	" *      * ",
	" *      * ",
	" *      * ",
	"  *    *  ",
	"   ****   ",
	"          "
    };

    for(i = 0; i < FONTSIZE; i++)
    {
	for(j = 0; j < FONTSIZE; j++)
	{
	    printf("%c", data_H[i][j]);
	}

	for(j = 0; j < FONTSIZE; j++)
	{
	    printf("%c", data_E[i][j]);
	}

	for(j = 0; j < FONTSIZE; j++)
	{
	    printf("%c", data_L[i][j]);
	}

	for(j = 0; j < FONTSIZE; j++)
	{
	    printf("%c", data_L[i][j]);
	}

	for(j = 0; j < FONTSIZE; j++)
	{
	    printf("%c", data_O[i][j]);
	}

	printf("\n");
    }

    return;
}


int main(int argc, char *argv[])
{
   while (1)
   { 
       print_hello();

       sleep(5);
   }

    return 0;
}
