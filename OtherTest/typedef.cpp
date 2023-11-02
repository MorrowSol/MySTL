#include<iostream>
using namespace std;


template <class T1, class T2>
struct base1
{
    typedef T1 first_type;
    typedef T2 second_type;
};

template <class T1, class T2>
struct sub1 : public base1<T1, T2>
{
    typename base1<T1, T2>::first_type a;
    typename base1<T1, T2>::second_type b;
    // 模板中需要再typedef一遍
    // first_type c;
    // second_type d;
};

struct base2
{
    typedef short first_type;
    typedef long second_type;
};
struct sub2 : public base2
{
    first_type a;
    second_type b;
};

int main()
{
    // sub1<short, long> s1;
    // sub2 s2;
    cout<<"success"<<endl;
    return 0;
}
