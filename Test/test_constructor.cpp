#include <iostream>

#include "allocator.h"
#include "gtest/gtest.h"
using namespace std;

class A1 {
public:
    static int call_constructor_num;
    static int call_destructor_num;
    // 构造析构里加点东西，变为不平凡
    A1() { call_constructor_num++; }
    ~A1() { call_destructor_num++; }
};
int A1::call_constructor_num = 0;
int A1::call_destructor_num = 0;
TEST(TestNoTriviallyConstructor, should_10_when_given_10) {
    int n = 10;
    // 测试类的构造析构
    mySTL::call_construct_num=0;
    mySTL::call_destruct_num=0;
    mySTL::Allocator<A1> alloc;
    A1* ptr = alloc.allocate(10);
    for (int i = 0; i < n; i++) {
        // 构造n次
        mySTL::construct(ptr + i);
    }
    // 批量析构一半
    mySTL::destroy(ptr, ptr + n/2);
    // 单独析构一半
    for(int i=n/2;i<n;i++){
        mySTL::destroy(ptr+n);
    }
    EXPECT_EQ(A1::call_constructor_num, n);
    EXPECT_EQ(A1::call_destructor_num, n);
    EXPECT_EQ(mySTL::call_construct_num, n);
    EXPECT_EQ(mySTL::call_destruct_num, n);
    alloc.deallocate(ptr);
}
TEST(TestTriviallyConstructor, should_0_when_given_10) {
    int n = 10;
    // 测试平凡类型
    mySTL::call_construct_num=0;
    mySTL::call_destruct_num=0;
    mySTL::Allocator<int> intalloc;
    int* ptr2 = intalloc.allocate(10);
    // for (int i = 0; i < n; i++) {
    //     // 构造n次
    //     mySTL::construct(ptr2 + i);
    // }
    // 批量析构
    // 批量析构一半
    mySTL::destroy(ptr2, ptr2 + n/2);
    // 单独析构一半
    for(int i=n/2;i<n;i++){
        mySTL::destroy(ptr2+n);
    }
    EXPECT_EQ(mySTL::call_construct_num, 0);
    EXPECT_EQ(mySTL::call_destruct_num, 0);
    intalloc.deallocate(ptr2);
}

