#include "gtest/gtest.h"
#include "vector.h"
#include <iostream>
#include <vecotr>
using namespace std;

TEST(test_expansion,should_run){
    mySTL::vector<int> v;
    // 初始容量
    EXPECT_EQ(v.size(),0);
    EXPECT_EQ(v.capacity(),0);
}