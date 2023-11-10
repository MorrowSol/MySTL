#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

// #include <new>
#include <type_traits>
#include <typeinfo>

#include "iterator.h"
#include "util.h"

namespace mySTL {

template <class T>
inline void __construct(T* ptr, std::true_type) {}
template <class T>
inline void __construct(T* ptr, std::false_type) {
    ::new ((void*)ptr) T();
}
template <class T>
inline void construct(T* ptr) {
    __construct(ptr,
                std::is_trivially_constructible<T>{});
}
template <class T, class V>
inline void construct(T* ptr, const V& value) {
    ::new ((void*)ptr) T(value);
}
template <class T, class... Args>
inline void construct(T* ptr, Args&&... args) {
    ::new ((void*)ptr) T(mySTL::forward<Args>(args)...);
}

template <class T>
inline void __destroy_one(T* pointer, std::true_type) {}
template <class T>
inline void __destroy_one(T* pointer, std::false_type) {
    if (pointer != nullptr) {
        pointer->~T();
    }
}
template <class T>
inline void destroy(T* pointer) {
    __destroy_one(pointer, std::is_trivially_destructible<T>{});
}

template <class ForwardIter>
inline void __destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
inline void __destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for (; first != last; ++first) destroy(&*first);
}
// 根据迭代器析构多个，通过类型萃取拿到类型，再判断类型
template <class ForwardIter>
inline void destroy(ForwardIter first, ForwardIter last) {
    __destroy_cat(
        first, last,
        std::is_trivially_destructible<
            typename mySTL::iterator_traits<ForwardIter>::value_type>{});
}

}  // namespace mySTL
#endif