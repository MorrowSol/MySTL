#include<iostream>
using namespace std;


template <class T,size_t size>
class A{
public:
    int v=size;
    int fun(int a){
        return a;
    }
    int fun2(){
        return fun(size);
    }
};

int main(){
    A<int,123> a;
    cout<<a.fun2()<<endl;
}