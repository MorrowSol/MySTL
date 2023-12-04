#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "vector.h"
using namespace std;

#ifdef MYTESTLOG
template <class T>
void printvector1d(const char* message, mySTL::vector<T>& v) {
    cout << message;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}
#endif
namespace testVector {

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

TEST(TestVector, Iterator) {
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

TEST(TestVector, PodInit) {
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

TEST(TestVector, NoPodInit) {
    // 初始化判断，非平凡类型
    int n = 2;
    mySTL::vector<A> v1;
    mySTL::vector<A> v2(n);
    A a;
    mySTL::vector<A> v3(n, a);
}

TEST(TestVector, EraseOne) {
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

TEST(TestVector, EraseRange) {
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

TEST(TestVector, PodVariableExpansion) {
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

TEST(TestVector, NoPodVariableExpansion) {
    mySTL::vector<A> v;
    A a;
    v.push_back(a);
    v.push_back(A());
    v.push_back(1,2,3);
}

TEST(TestVector,InsertOne){
    int n=10;
    mySTL::vector<int> v(n);
    for(int i=0;i<n;i++){
        v[i]=i;
    }
#ifdef MYTESTLOG
    printvector1d("初始化: ", v);
#endif    
    v.insert(v.begin(),111);
    EXPECT_EQ(int(v.size()),n+1);
    EXPECT_EQ(v[0], 111);
#ifdef MYTESTLOG
    printvector1d("插在头部 : ", v);
#endif
    v.insert(v.begin()+4,222);
    EXPECT_EQ(v[4], 222);
    EXPECT_EQ(int(v.size()),n+2);
#ifdef MYTESTLOG
    printvector1d("插在中部4: ", v);
#endif
    v.insert(v.end(),333);
    EXPECT_EQ(v[v.size()-1], 333);
    EXPECT_EQ(int(v.size()),n+3);
#ifdef MYTESTLOG
    printvector1d("插在尾部 : ", v);
#endif 
}

TEST(TestVector,InsertN){
    int n=10;
    int N = 3;
    mySTL::vector<int> v(n);
    for(int i=0;i<n;i++){
        v[i]=i;
    }
#ifdef MYTESTLOG
    printvector1d("初始化: ", v);
#endif    
    v.insert(v.begin(),N,111);
    EXPECT_EQ(int(v.size()),n+N);
    EXPECT_EQ(v[0], 111);
#ifdef MYTESTLOG
    printvector1d("插在头部 : ", v);
#endif
    v.insert(v.begin()+4,N,222);
    EXPECT_EQ(v[4], 222);
    EXPECT_EQ(int(v.size()),n+N+N);
#ifdef MYTESTLOG
    printvector1d("插在中部4: ", v);
#endif

    v.insert(v.begin()+4,20,444);
    EXPECT_EQ(v[4], 444);
    EXPECT_EQ(int(v.size()),n+N+N+20);
#ifdef MYTESTLOG
    printvector1d("插在中部并扩容: ", v);
#endif

    v.insert(v.end(),N,333);
    EXPECT_EQ(v[v.size()-1], 333);
    EXPECT_EQ(int(v.size()),n+N+N+N+20);
#ifdef MYTESTLOG
    printvector1d("插在尾部 : ", v);
#endif 
}

TEST(TestVector,Resize){
    int n=10;
    mySTL::vector<int> v(n);
    for(int i=0;i<n;i++){
        v[i]=i;
    }
#ifdef MYTESTLOG
    printvector1d("初始化  : ", v);
#endif
    v.resize(5);
    EXPECT_EQ(int(v.size()), 5);
#ifdef MYTESTLOG
    printvector1d("resize 5: ", v);
#endif
    v.resize(100);
    EXPECT_EQ(int(v.size()), 100);
#ifdef MYTESTLOG
    printvector1d("resize 100: ", v);
#endif
}

TEST(TestVector,Resize2){
    mySTL::vector<int> v1;
    EXPECT_EQ(int(v1.size()),0);
    EXPECT_EQ(int(v1.capacity()),0);
    v1.resize(10,1);
    EXPECT_EQ(int(v1.size()),10);
    EXPECT_EQ(int(v1.capacity()),10);
}


}  // namespace testVector