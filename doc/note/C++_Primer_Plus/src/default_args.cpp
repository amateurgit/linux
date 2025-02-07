#include <iostream>

using namespace std;

char *left(const char * str, int n = 1);  /* 设置默认参数 n = 1 */

int main() {

    char *pstr = left("Hello");

    cout << pstr << endl;

    delete [] pstr;

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
