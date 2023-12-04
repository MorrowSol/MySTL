#ifndef _ALGO_H_
#define _ALGO_H_

#include "iterator.h"

namespace mySTL {

template <class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
    typedef typename mySTL::iterator_traits<ForwardIterator>::difference_type difference_type;
    difference_type len = mySTL::distance(first, last);
    while (len > 0) {
        difference_type half = len >> 1;
        ForwardIterator middle = first;
        mySTL::advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

template<class T>
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}
}  // namespace mySTL
#endif