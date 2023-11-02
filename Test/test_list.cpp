#include <iostream>

#include "gtest/gtest.h"
#include "list.h"
// #include <list>
using namespace std;


namespace testList {

#ifdef MYTESTLOG
template <class T>
void printlist(const char* message, mySTL::list<T>& list) {
    cout << message;
    for(auto it = list.begin();it!=list.end();++it){
        cout<<*it<<" ";
    }
    cout << endl;
}
#endif

class A{
public:
    int a;
    A(){}
    A(int a):a(a){}
    A& operator=(const A& x){
        a= x.a;
        return *this;
    }
    friend ostream & operator<<(ostream & out, A & x);
};
//重载输出运算符
ostream & operator<<(ostream & out, A & x){
    out << x.a;
    return out;
}
TEST(TestListIterator, OperatorOverload) {
    mySTL::__list_node<A> node1;
    node1.data = 123;
    node1.prev = nullptr;
    node1.next = nullptr;
    mySTL::__list_node<A> node2;
    node2.data = 456;
    node2.prev = nullptr;
    node2.next = nullptr;

    mySTL::__list_iterator<A> it1(&node1);
    mySTL::__list_iterator<A> it2(&node1);
    mySTL::__list_iterator<A> it3(&node2);
    EXPECT_EQ(it1==it2, true);
    EXPECT_EQ(it1!=it3, true);
    EXPECT_EQ((*it1).a, 123);
    EXPECT_EQ(it1->a, 123);
}

TEST(TestList, ConstructAndIterator) {
    // 默认构造 平凡类型
    mySTL::list<int> mylist;
    EXPECT_EQ(mylist.empty(), true);
    EXPECT_EQ(int(mylist.size()), 0);
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);
    mylist.push_back(4);
    EXPECT_EQ(mylist.empty(), false);
    EXPECT_EQ(int(mylist.size()), 4);
    EXPECT_EQ(*mylist.begin(), 1);
    EXPECT_EQ(*(--mylist.end()), 4);
    EXPECT_EQ(mylist.front(), 1);
    EXPECT_EQ(mylist.back(), 4);
#ifdef MYTESTLOG 
    printlist("平凡类型初始化: ",mylist);
#endif

    // 类 类型
    mySTL::list<A> mylist2;
    EXPECT_EQ(mylist2.empty(), true);
    EXPECT_EQ(int(mylist2.size()), 0);
    // 这里push时应该会强转
    mylist2.push_back(1);
    mylist2.push_back(2);
    mylist2.push_back(3);
    mylist2.push_back(4);
    EXPECT_EQ(mylist2.empty(), false);
    EXPECT_EQ(int(mylist2.size()), 4);
    EXPECT_EQ((*mylist2.begin()).a, 1);
    EXPECT_EQ((*(--mylist2.end())).a, 4);
    EXPECT_EQ(mylist2.front().a, 1);
    EXPECT_EQ(mylist2.back().a, 4);
#ifdef MYTESTLOG 
    printlist("类类型初始化: ",mylist2);
#endif
}

TEST(TestList, PushAndErase) {
    // 默认构造
    mySTL::list<int> mylist;
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);
    mylist.push_back(4);
    mylist.push_back(5);
    EXPECT_EQ(int(mylist.size()), 5);
#ifdef MYTESTLOG
    printlist("初始化: ",mylist);
#endif
    mylist.push_front(0);
    mylist.push_front(-1);
    EXPECT_EQ(int(mylist.size()), 7);
    EXPECT_EQ(mylist.front(), -1);
#ifdef MYTESTLOG
    printlist("push_front: ",mylist);
#endif
    mylist.pop_back();
    mylist.pop_front();
    EXPECT_EQ(int(mylist.size()), 5);
    EXPECT_EQ(mylist.back(), 4);
#ifdef MYTESTLOG
    printlist("pop_front_back: ",mylist);
#endif    
    mylist.insert(mylist.begin()+4,2);
    EXPECT_EQ(*(mylist.begin()+4), 2);
#ifdef MYTESTLOG
    printlist("insert pos 4: ",mylist);
#endif
    mylist.erase(mylist.begin()+1);
    EXPECT_EQ(*(mylist.begin()+1), 2);
#ifdef MYTESTLOG
    printlist("erase pos 1: ",mylist);
#endif
    mylist.remove(2);
#ifdef MYTESTLOG
    printlist("remove 2: ",mylist);
#endif    
    mylist.clear();
    EXPECT_EQ(int(mylist.size()), 0);

}

TEST(TestList, Transfer) {
    // 默认构造
    mySTL::list<int> mylist;
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);
    mylist.push_back(4);
    mylist.push_back(5);
    mySTL::list<int> mylist2;
    mylist2.push_back(6);
    mylist2.push_back(7);
    mylist2.push_back(8);
    mylist2.push_back(9);
    mylist2.push_back(10);
#ifdef MYTESTLOG
    printlist("list1 : ",mylist);
    printlist("list12: ",mylist2);
#endif 
    mylist.transfer(mylist.begin()+2,mylist2.begin()+1,mylist2.begin()+4);
#ifdef MYTESTLOG
    cout<<"transfer后"<<endl;
    printlist("list1 : ",mylist);
    printlist("list12: ",mylist2);
#endif     
    EXPECT_EQ(int(mylist.size()), 8);
    EXPECT_EQ(int(mylist2.size()), 2);
    EXPECT_EQ(*(mylist.begin()+2), 7);
    EXPECT_EQ(*(mylist2.begin()+1), 10);
}




    
    


}  // namespace testList