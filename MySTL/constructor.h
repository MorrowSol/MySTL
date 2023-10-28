#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

// #include <new>
#include <type_traits>

#include "iterator.h"
#include "util.h"
#include <iostream>
#include <typeinfo>

namespace mySTL {

static int call_construct_num=0;
static int call_destruct_num=0;

template <class T>
void construct(T* ptr) {
    ::new ((void*)ptr) T();
    call_construct_num++;
}
template <class T, class V>
void construct(T* ptr, const V& value) {
    ::new ((void*)ptr) T(value);
    call_construct_num++;
}
template <class T, class... Args>
void construct(T* ptr, Args&&... args) {
    ::new ((void*)ptr) T(mySTL::forward<Args>(args)...);
    call_construct_num++;
}


template <class T>
void destroy_one(T* pointer,std::true_type) {}
template <class T>
void destroy_one(T* pointer,std::false_type) {
    if (pointer != nullptr) {
        pointer->~T();
        call_destruct_num++;
    }
}
template <class T>
void destroy(T* pointer) {
    destroy_one(pointer,std::is_trivially_destructible<T>{});
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for (; first != last; ++first) destroy(&*first);
}
// 根据迭代器析构多个，通过类型萃取拿到类型，再判断类型
template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last) {
    destroy_cat(
        first, last,
        std::is_trivially_destructible<
            typename mySTL::iterator_traits<ForwardIter>::value_type>{});
}

}  // namespace mySTL
#endif