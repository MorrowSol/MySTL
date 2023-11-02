#include<iostream>
using namespace std;

template <class T>
class A{
public:
    T data;
    template <class U>
    struct rebind
    {
        typedef A<U> other;
    };
    
};

int main(){
    A<int> a;
    a.data = 123;
    A<int>::rebind<int>::other b;
    b.data=1.23;

    cout<<a.data<<endl;
    cout<<b.data<<endl;
    return 0;
}