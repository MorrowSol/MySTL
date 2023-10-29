#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "vector.h"
using namespace std;

#ifdef MYTESTLOG
template <class T>
void printvector1d(const char* message, mySTL::vector<T>& v) {
    cout << message;
    int n=v.size();
    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

#endif

class A {
public:
    A() {
#ifdef MYTESTLOG
        cout << "无参构造" << endl;
#endif
    }
    A(int a, int b, int c) {
#ifdef MYTESTLOG
        cout << "有参构造" << endl;
#endif
    }
    A(const A& x) {
#ifdef MYTESTLOG
        cout << "拷贝构造" << endl;
#endif
    }
    A(A&& x) {
#ifdef MYTESTLOG
        cout << "移动构造" << endl;
#endif
    }
};

TEST(test_vector_iterator, should_runok) {
    // 迭代器判断
    mySTL::vector<int> v;
    // 初始容量
    EXPECT_EQ(int(v.size()), 0);
    EXPECT_EQ(int(v.capacity()), 0);
    EXPECT_EQ(v.empty(), true);
    v.push_back(1);
    EXPECT_EQ(int(v.size()), 1);
    EXPECT_EQ(int(v.capacity()), 1);
    EXPECT_EQ(v.empty(), false);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 1);
    EXPECT_EQ(*v.begin(), 1);
    EXPECT_EQ(*(v.end() - 1), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(test_vector_pod_init, should_runok) {
    // 初始化判断，平凡类型
    mySTL::vector<int> v1;
    EXPECT_EQ(int(v1.size()), 0);
    EXPECT_EQ(int(v1.capacity()), 0);
    EXPECT_EQ(v1.empty(), true);

    int n = 10;
    mySTL::vector<int> v2(n);
    EXPECT_EQ(int(v2.size()), n);
    EXPECT_EQ(int(v2.capacity()), n);

    int value = 10;
    mySTL::vector<int> v3(n, value);
    for (int i = 0; i < n; i++) EXPECT_EQ(v3[i], value);
}

TEST(test_vector_no_pod_init, should_runok) {
    // 初始化判断，非平凡类型
    int n=10;
    mySTL::vector<A> v1;
    mySTL::vector<A> v2(n);
    A a;
    mySTL::vector<A> v3(n,a);
    
}

TEST(test_vector_erase_one, should_runok) {
    // 删除一个（头部，中部，尾部）
    int n = 10;
    mySTL::vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
#ifdef MYTESTLOG
    printvector1d("原数组：", v);
#endif
    // 头部
    v.erase(v.begin());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(int(v.size()), n - 1);
    EXPECT_EQ(int(v.capacity()), n);

#ifdef MYTESTLOG
    printvector1d("删头部：", v);
#endif

    // 中部，删除5
    v.erase(v.begin() + 4);
    EXPECT_EQ(v[4], 6);
    EXPECT_EQ(int(v.size()), n - 2);

#ifdef MYTESTLOG
    printvector1d("删中部：", v);
#endif

    // 尾部
    v.erase(v.end() - 1);
    EXPECT_EQ(*(v.end() - 1), n - 2);
    EXPECT_EQ(int(v.size()), n - 3);
#ifdef MYTESTLOG
    printvector1d("删尾部：", v);
#endif
}

TEST(test_vector_erase_range, should_runok) {
    // 删除多个（头部，中部，尾部）

    int n = 20;
    mySTL::vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
#ifdef MYTESTLOG
    printvector1d("原数组：", v);
#endif
    // 头部
    v.erase(v.begin(), v.begin() + 2);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(int(v.size()), n - 2);
    EXPECT_EQ(int(v.capacity()), n);

#ifdef MYTESTLOG
    printvector1d("删头部：", v);
#endif

    // 中部，删除5,6,7
    v.erase(v.begin() + 3, v.begin() + 6);
    EXPECT_EQ(v[3], 8);
    EXPECT_EQ(int(v.size()), n - 2 - 3);

#ifdef MYTESTLOG
    printvector1d("删中部：", v);
#endif

    // 尾部
    v.erase(v.end() - 5, v.end());
    EXPECT_EQ(*(v.end() - 1), n - 6);
    EXPECT_EQ(int(v.size()), n - 2 - 3 - 5);
#ifdef MYTESTLOG
    printvector1d("删尾部：", v);
#endif

    v.clear();
    EXPECT_EQ(int(v.size()), 0);
    EXPECT_EQ(int(v.capacity()), n);
#ifdef MYTESTLOG
    printvector1d("清空：", v);
#endif
}

TEST(test_vector_pod_expansion, should_runok) {
    mySTL::vector<int> v;
    EXPECT_EQ(int(v.size()), 0);
    EXPECT_EQ(int(v.capacity()), 0);
    v.push_back(1);
    EXPECT_EQ(int(v.size()), 1);
    EXPECT_EQ(int(v.capacity()), 1);
    v.push_back(2);
    EXPECT_EQ(int(v.size()), 2);
    EXPECT_EQ(int(v.capacity()), 2);
    v.push_back(3);
    EXPECT_EQ(int(v.size()), 3);
    EXPECT_EQ(int(v.capacity()), 4);
    v.push_back(4);
    v.push_back(5);
    EXPECT_EQ(int(v.size()), 5);
    EXPECT_EQ(int(v.capacity()), 8);
#ifdef MYTESTLOG
    printvector1d("push(1,2,3,4,5)后: ", v);
#endif
}

TEST(test_vector_push_back, should_runok) {
    // mySTL::vector<A> v;
    // A a;
    // v.push_back(a);
    // v.push_back(A());
}
