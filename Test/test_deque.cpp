#include <iostream>
#include "deque.h"
#include "gtest/gtest.h"
#include <deque>
using namespace std;

namespace testDeque {

#ifdef MYTESTLOG
template <class T>
void printdeque(const char* message, T& dq) {
    cout << message;
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        // cout << it.node<<" "<<it.cur << " "<<endl;
        cout << *it << " ";
    }
    cout << endl;
}
#endif

class A {
public:
    int a;
    A() {}
    A(int a) : a(a) {}
    A& operator=(const A& x) {
        a = x.a;
        return *this;
    }
    friend ostream& operator<<(ostream& out, A& x);
};
// 重载输出运算符
ostream& operator<<(ostream& out, A& x) {
    out << x.a;
    return out;
}

TEST(TestDeque, InitAndIterator) {
    mySTL::deque<int> dq;
    mySTL::deque<int> dq2(10);
    mySTL::deque<int> dq3(10, 2);

#ifdef MYTESTLOG
    printdeque("无参构造: ", dq);
    printdeque("有参构造1: ", dq2);
    printdeque("有参构造2: ", dq3);
#endif
    EXPECT_EQ(dq.empty(), true);
    EXPECT_EQ(dq2.empty(), false);
    EXPECT_EQ(int(dq.size()), 0);
    EXPECT_EQ(int(dq2.size()), 10);
    EXPECT_EQ(*(dq3.begin()), 2);
    EXPECT_EQ(*(dq3.end() - 1), 2);
    EXPECT_EQ(dq3.front(), 2);
    EXPECT_EQ(dq3.back(), 2);
}

TEST(TestDeque, Expansion) {
    // 这里控制节点大小为四个int，便于触发扩容
    mySTL::deque<int, mySTL::Allocator<int>, 4> dq(10, 12);
#ifdef MYTESTLOG
    printdeque("原dp: ", dq);
#endif
    dq.printinfo();
    for (int i = 0; i < 10; i++) dq.push_front(i);
    EXPECT_EQ(dq.front(), 9);
    for (int i = 0; i < 10; i++) 
        dq.push_back(i);
    EXPECT_EQ(dq.back(), 9);
    EXPECT_EQ(int(dq.size()), 30);
    dq.printinfo();
#ifdef MYTESTLOG
    printdeque("现dp: ", dq);
#endif
    for (int i = 0; i < 50; i++) dq.push_front(i);
    EXPECT_EQ(dq.front(), 49);
    for (int i = 0; i < 50; i++) dq.push_back(i);
    EXPECT_EQ(dq.back(), 49);
    dq.printinfo();
    EXPECT_EQ(int(dq.size()), 130);
#ifdef MYTESTLOG
    printdeque("现dp: ", dq);
#endif
    for (int i = 0; i < 50; i++) {
        dq.pop_back();
    }
    for (int i = 0; i < 50; i++) {
        dq.pop_front();
    }
    EXPECT_EQ(dq.front(), 9);
    EXPECT_EQ(dq.back(), 9);
    EXPECT_EQ(int(dq.size()), 30);
#ifdef MYTESTLOG
    printdeque("pop后dp: ", dq);
#endif
    dq.clear();
    EXPECT_EQ(int(dq.size()), 0);
#ifdef MYTESTLOG
    printdeque("clear后: ", dq);
#endif
}

TEST(TestDeque, Erase) {
    mySTL::deque<int, mySTL::Allocator<int>, 4> dq;
    for(int i=0;i<30;i++){
        dq.push_back(i);
    }
#ifdef MYTESTLOG
    printdeque("原dp: ", dq);
#endif
    dq.erase(dq.begin()+10,dq.begin()+20);
    EXPECT_EQ(int(dq.size()), 20);
    EXPECT_EQ(*(dq.begin()+10), 20);
#ifdef MYTESTLOG
    printdeque("erase后: ", dq);
#endif
    dq.erase(dq.begin()+10);
    dq.erase(dq.begin()+10);
    dq.erase(dq.begin()+10);
    EXPECT_EQ(int(dq.size()), 17);
    EXPECT_EQ(*(dq.begin()+10), 23);
#ifdef MYTESTLOG
    printdeque("erase后: ", dq);
#endif
    deque<int> dq2;
    dq2.emplace_back();
}   

TEST(TestDeque, Insert) {
    mySTL::deque<int, mySTL::Allocator<int>, 4> dq;
    for(int i=0;i<15;i++){
        dq.push_back(i);
    }
#ifdef MYTESTLOG
    printdeque("原dp: ", dq);
#endif
    dq.insert(dq.begin()+10,123);
    EXPECT_EQ(*(dq.begin()+10), 123);
    EXPECT_EQ(int(dq.size()), 16);
#ifdef MYTESTLOG
    printdeque("插入后: ", dq);
#endif
    dq.insert(dq.begin()+13,14,234);
    EXPECT_EQ(*(dq.begin()+13), 234);
    EXPECT_EQ(int(dq.size()), 30);
#ifdef MYTESTLOG
    printdeque("插入后: ", dq);
#endif
}







}  // namespace testDeque