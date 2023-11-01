#include <iostream>

#include "allocator.h"
#include "gtest/gtest.h"
using namespace std;

class A1 {
public:
    static int call_constructor_num;
    static int call_destructor_num;
    int a;
    int b;
    // 构造析构里加点东西，变为不平凡
    A1() { call_constructor_num++; }
    A1(int a):a(a) { call_constructor_num++; }
    A1(int a,int b):a(a),b(b) { call_constructor_num++; }
    ~A1() { call_destructor_num++; }
};
int A1::call_constructor_num = 0;
int A1::call_destructor_num = 0;
TEST(TestConstructor, NoPodVariable) {
    int n = 10;
    // 测试类的构造析构
    mySTL::Allocator<A1> alloc;
    A1* ptr = alloc.allocate(10);
    // 构造1/3次无参
    for (int i = 0; i < n/3; i++) {
        mySTL::construct(ptr + i);
    }
    // 构造1/3次有一个参数
    for (int i = n/3; i < n/3*2; i++) {
        mySTL::construct(ptr + i,i);
    }
    // 构造1/3次有两个参数
    for (int i = n/3*2; i < n; i++) {
        mySTL::construct(ptr + i,i,i);
    }
    // 批量析构一半
    mySTL::destroy(ptr, ptr + n/2);
    // 单独析构一半
    for(int i=n/2;i<n;i++){
        mySTL::destroy(ptr+n);
    }
    EXPECT_EQ(A1::call_constructor_num, n);
    EXPECT_EQ(A1::call_destructor_num, n);
    alloc.deallocate(ptr);
}
TEST(TestConstructor, PodVariable) {
    int n = 10;
    // 测试平凡类型
    mySTL::Allocator<int> intalloc;
    int* ptr2 = intalloc.allocate(10);
    // 构造1/2次无参
    for (int i = 0; i < n/2; i++) {
        mySTL::construct(ptr2 + i);
    }
    // 构造1/2次有一个参数
    for (int i = n/2; i < n; i++) {
        mySTL::construct(ptr2 + i,i);
    }
    // 批量析构
    // 批量析构一半
    mySTL::destroy(ptr2, ptr2 + n/2);
    // 单独析构一半
    for(int i=n/2;i<n;i++){
        mySTL::destroy(ptr2+n);
    }
    intalloc.deallocate(ptr2);
}

