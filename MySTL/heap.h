#ifndef _HEAP_H_
#define _HEAP_H_

#include <iterator.h>

namespace mySTL {

// push 
template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last, Distance *, T *) {
    __push_heap(first, Distance((last - first) - 1), Distance(0),
                T(*(last - 1)));
}
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}


template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len,
                   T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}
template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                       RandomAccessIterator result, T value, Distance *) {
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}
template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last, T*) {
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __pop_heap_aux(first, last, value_type(first));
}


template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,RandomAccessIterator last){
    while (last-first>1){
        pop_heap(first, last--);
    }
}
template<class RandomAccessIterator,class T, class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*){
    if(last-first<2)return;
    Distance len = last-first;
    Distance parent = (len-2)/2;
    while(true){
        __adjust_heap(first,parent,len,T(*(first+parent)));
        if(parent==0)return;
        parent--;
    }
}
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,RandomAccessIterator last){
    __make_heap(first, last,value_type(first),distance_type(last));
}

// void max_heapify(int arr[], int start, int end) {
//     // 建立父节点指标和子节点指标
//     int dad = start;
//     int son = dad * 2 + 1;
//     while (son <= end) {  // 若子节点指标在范围内才做比较
//         if (son + 1 <= end &&
//             arr[son] < arr[son + 1])  // 先比较两个子节点大小，选择最大的
//             son++;
//         if (arr[dad] >
//             arr[son])  // 如果父节点大于子节点代表调整完毕，直接跳出函数
//             return;
//         else {  // 否则交换父子内容再继续子节点和孙节点比较
//             swap(&arr[dad], &arr[son]);
//             dad = son;
//             son = dad * 2 + 1;
//         }
//     }
// }
// void heap_sort(int arr[], int len) {
//     int i;
//     // 初始化，i从最后一个父节点开始调整
//     for (i = len / 2 - 1; i >= 0; i--) max_heapify(arr, i, len - 1);
//     // 先将第一个元素和已排好元素前一位做交换，再从新调整，直到排序完毕
//     for (i = len - 1; i > 0; i--) {
//         swap(&arr[0], &arr[i]);
//         max_heapify(arr, 0, i - 1);
//     }
// }
}  // namespace mySTL
#endif