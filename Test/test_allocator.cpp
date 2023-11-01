#include "allocator.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>
using namespace std;


TEST(TestAllocator,AllocateAndDeallocate){
    class A {
        int a;
        int b;

    public:
        A() : a(0), b(0) {}
        A(int a) : a(a),b(0) {}
        A(int a, int b) : a(a), b(b) {}
        int geta() {return a;}
        int getb() {return b;}
    };
    // 申请一个int
    mySTL::Allocator<int> intAllocator;
    int* intPointer = intAllocator.allocate(1);  // 分配一个int类型的对象
    intAllocator.deallocate(intPointer, 1);  // 释放int类型的对象
    // 申请一个class空间，并构造
    mySTL::Allocator<A> AAllocator;
    A* APointer = AAllocator.allocate();
    AAllocator.construct(APointer);
    EXPECT_EQ(APointer->geta(),0);
    EXPECT_EQ(APointer->getb(),0);
    int tmp = 123;
    AAllocator.construct(APointer, tmp);
    EXPECT_EQ(APointer->geta(),123);
    EXPECT_EQ(APointer->getb(),0);
    AAllocator.construct(APointer, 456);
    EXPECT_EQ(APointer->geta(),456);
    EXPECT_EQ(APointer->getb(),0);
    AAllocator.construct(APointer, 123, 456);
    EXPECT_EQ(APointer->geta(),123);
    EXPECT_EQ(APointer->getb(),456);
    // 析构
    AAllocator.destroy(APointer);
    // 释放空间
    AAllocator.deallocate(APointer);

    // 申请多个class空间，并构造
    mySTL::Allocator<A> alloctor;
    A* AAPointer = alloctor.allocate(10);
    for(int i=0;i<10;i++){
        alloctor.construct(AAPointer+i,i);
    }
    for(int i=0;i<10;i++){
        EXPECT_EQ((AAPointer+i)->geta(),i);
    }
    // 析构
    for(int i=0;i<10;i++){
        AAllocator.destroy(AAPointer+i);
    }
    // 释放空间
    AAllocator.deallocate(AAPointer);
}

TEST(TestAllocator,StlVectorUse){
    int a[]={1,2,3,4,5};
    vector<int,mySTL::Allocator<int>> v1(a,a+5);
    for(int i=0;i<5;i++){
        EXPECT_EQ(v1[i],a[i]);
    }  

    vector<int,mySTL::Allocator<int>> v2;
    v2.push_back(0);
    EXPECT_EQ(v2[0],0);
}