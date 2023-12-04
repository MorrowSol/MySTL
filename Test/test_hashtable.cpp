#include <functional>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "hashtable.h"
using namespace std;
namespace testHashTable {

#ifdef MYTESTLOG
template <class T>
void printvector(const char* message, T& dq) {
    cout << message;
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        // cout << it.node<<" "<<it.cur << " "<<endl;
        cout << *it << " ";
    }
    cout << endl;
}
#endif

template <class T>
class identity {
public:
    T operator()(const T& t) const { return t; }
};

// 测试RBTree的insert函数
TEST(TestHashTable, InsertTest) {
    // 创建一个RBTree对象
    std::hash<int> hashfunc;
    std::equal_to<int> eqfunc;
    identity<int> idfunc;
    mySTL::hashtable<int, int, std::hash<int>, identity<int>, std::equal_to<int>> ht(
        1, hashfunc, eqfunc, idfunc);

    EXPECT_EQ(int(ht.size()), 0);
    EXPECT_EQ(int(ht.bucket_count()), 53);

    ht.insert_unique(59);
    ht.insert_unique(63);
    ht.insert_unique(108);
    ht.insert_unique(2);
    ht.insert_unique(53);
    ht.insert_unique(55);

    EXPECT_EQ(int(ht.size()), 6);

    mySTL::hashtable<int, int, std::hash<int>, identity<int>,
                     std::equal_to<int>>::iterator it = ht.begin();
    int n = ht.size();
    std::vector<int> res;
    for (int i = 0; i < n; i++, ++it) {
        res.push_back(*it);
    }
#ifdef MYTESTLOG
    printvector("打印hashtable: ", res);
#endif
    // 0:  53
    // 2:  55 2 108            2位置开链
    // 6:  59
    // 10: 63 
    EXPECT_THAT(res, ::testing::ElementsAreArray({53, 55, 2, 108, 59, 63})); 
}

}  // namespace testHashTable