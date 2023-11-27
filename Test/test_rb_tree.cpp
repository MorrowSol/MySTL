#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "rb_tree.h"

using namespace std;
namespace testRbTree {

template <class T>
class identity {
public:
    T operator()(const T& t) const { return t; }
};

// 测试RBTree的insert函数
TEST(RBTreeTest, InsertTest) {
    // 创建一个RBTree对象
    mySTL::rb_tree<int, int, identity<int>, less<int>> rb_tree;

    // 往RBTree中插入一些值
    rb_tree.insert_unique(5);
    rb_tree.insert_unique(3);
    rb_tree.insert_unique(7);
    rb_tree.insert_unique(1);
    rb_tree.insert_unique(4);

    // 断言RBTree的大小是否正确
    EXPECT_EQ(int(rb_tree.size()), 5);
}
TEST(RbTreeTest, Find) {
    // 创建一个RBTree对象
    mySTL::rb_tree<int, int, identity<int>, less<int>> rb_tree;
    rb_tree.insert_unique(5);
    rb_tree.insert_unique(3);
    rb_tree.insert_unique(7);

    EXPECT_EQ(*rb_tree.find(5), 5);
    EXPECT_EQ(*rb_tree.find(3), 3);
    EXPECT_EQ(*rb_tree.find(7), 7);
    EXPECT_EQ(rb_tree.find(2), rb_tree.end());
}

TEST(RbTreeTest, TraverseInOrder) {
    mySTL::rb_tree<int, int, identity<int>, less<int>> rb_tree;
    rb_tree.insert_unique(5);
    rb_tree.insert_unique(3);
    rb_tree.insert_unique(7);
    rb_tree.insert_unique(1);
    rb_tree.insert_unique(2);
    rb_tree.insert_unique(6);
    rb_tree.insert_unique(10);
    rb_tree.insert_unique(4);
    rb_tree.insert_unique(8);
    rb_tree.insert_unique(9);

    EXPECT_EQ(int(rb_tree.size()), 10);
    EXPECT_THAT(rb_tree.traverseInOrder(),
                ::testing::ElementsAreArray({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

}  // namespace testRbTree