#ifndef _ITERATOR_H
#define _ITERATOR_H

#include <cstddef>

namespace mySTL {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirection_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirection_iterator_tag {};

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

// 类型萃取

template <typename Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};
template <typename _Tp>
struct iterator_traits<_Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};

/// Partial specialization for const pointer types.
template <typename _Tp>
struct iterator_traits<const _Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef const _Tp* pointer;
    typedef const _Tp& reference;
};
// 获取迭代器类型，值类型，diff类型函数
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(
    const Iterator&) {
    typedef typename iterator_traits<Iterator>::value_type* type;
    return static_cast<type>(0);
}
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(
    const Iterator&) {
    typedef typename iterator_traits<Iterator>::difference_type* type;
    return static_cast<type>(0);
}

// distance
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type __distance(
    Iterator first, Iterator last, random_access_iterator_tag) {
    return last - first;
}
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type __distance(
    Iterator first, Iterator last, input_iterator_tag) {
    typename iterator_traits<Iterator>::difference_type dis = 0;
    while (first != last) {
        ++first;
        ++dis;
    }
    return dis;
}
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type distance(
    Iterator first, Iterator last) {
    __distance(first, last, iterator_category(first));
}

// advance
template <class Iterator, class Distance>
inline void advance(Iterator& iter, Distance n, input_iterator_tag) {
    while (n--) ++iter;
}
template <class Iterator, class Distance>
inline void advance(Iterator& iter, Distance n, bidirection_iterator_tag) {
    if (n >= 0) {
        while (n--) ++iter;
    } else {
        while (n++) --iter;
    }
}
template <class Iterator, class Distance>
inline void advance(Iterator& iter, Distance n, random_access_iterator_tag) {
    iter += n;
}
template <class Iterator, class Distance>
inline void advance(Iterator& iter, Distance n) {
    __advance(iter, n, iterator_category(iter));
}

}  // namespace mySTL
#endif