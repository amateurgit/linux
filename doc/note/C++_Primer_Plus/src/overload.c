#include <iostream>

using namespace std;

char *left(const char * str, int n = 1);  /* 设置默认参数 n = 1 */
unsigned long left(unsigned long num, unsigned ct);

int main() {

    char *pstr = NULL;
    unsigned long num = 12345;

    pstr = left("Hello");
    cout << pstr << endl;

    delete [] pstr;

    unsigned long ret = left(num, 3);
    cout << ret << endl;

    return 0;

}

char *left( const char * str, int n ) {

    if ( n < 0 )
        n = 0;

    char *p = new char[n + 1];
    int i;

    for (i = 0; i < n && str[i]; i++) {
        p[i] = str[i];
    }

    while ( i <= n ) {
        p[i++] = '\0';
    }

    return p;

}

unsigned long left(unsigned long num, unsigned ct) {

    unsigned digits = 1;
    unsigned long n = num;

    if ( 0 == ct || 0 == num )
        return 0;

    while (n /= 10)  /* 位数 */
        digits++;

    if (digits > ct) {
        ct = digits - ct;
        while (ct--)
            num /= 10;
    } 

    return num;

}
