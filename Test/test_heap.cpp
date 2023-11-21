#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "heap.h"
using namespace std;
namespace testHeap {

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
TEST(TestHeap, make) {
    int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    vector<int> ivec(ia, ia + 9);
    mySTL::make_heap(ivec.begin(), ivec.end());
    printvector("make: ",ivec);
    
    ivec.push_back(7);
    mySTL::push_heap(ivec.begin(),ivec.end());
    printvector("push 7: ",ivec); 

    mySTL::pop_heap(ivec.begin(),ivec.end());
    cout<<ivec.back()<<endl;
    ivec.pop_back();
    printvector("pop 后: ",ivec);

    mySTL::sort_heap(ivec.begin(),ivec.end());
    printvector("sort 后: ",ivec);
}

}