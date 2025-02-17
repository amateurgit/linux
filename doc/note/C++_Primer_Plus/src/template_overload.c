#include <iostream>

using namespace std;

template <typename T>
void Swap(T &a, T &b);

template <typename T>
void Swap(T *a, T *b, int n);

int main() {

    /* 业务代码.......  */

    return 0;
}

template <typename T>
void Swap(T &a, T &b) {
    T tmp;

    tmp = a;
    a = b;
    b = tmp;

    return;
}

template <typename T>
void Swap(T *a, T *b, int n) {

    T temp;

    int i = 0;

    for (i = 0; i < n; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i]; = temp;
    }

}
