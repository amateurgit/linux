#include <iostream>

using namespace std;

int main() {

    int a = 10;

    int & age = a; 

    cout << age << endl;

    cout << " a 的地址 " << &a << endl;
    cout << " age 的地址 " << &age  << endl;

    return 0;

}
