#include <iostream>

using namespace std;

template <typename T>
void Swap(T &a, T &b);

int main() {

    int i = 10;
    int j = 20;

    cout << "i, j = " << i << ", " << j << endl;
    cout << "调用模版交换数据 : " << endl;
    Swap(i, j);
    cout << "i, j = " << i << ", " << j << endl;

    double x = 3.14;
    double y = 6.28;

    cout << "x, y = " << x << ", " << y << endl;
    cout << "调用模版交换数据 : " << endl;
    Swap(x, y);
    cout << "x, y = " << x << ", " << y << endl;

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
