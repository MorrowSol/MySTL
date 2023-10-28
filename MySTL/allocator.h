#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

// 这个头文件包含一个模板类
// allocator，用于管理内存的分配、释放，对象的构造、析构
// #include <iostream>
#include "constructor.h"
// size_t ptrdiff_t
#include <cstddef>
// exit
#include <stdlib.h>

namespace mySTL {

// 模板类：allocator
// 根据STL规范，需要实现必要接口
// 其中construct系，实际使用没有用到，在c++20中被删除
template <class T>
class Allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // Type to represent another allocator for a different type
    template <class U>
    struct rebind {
        typedef Allocator<U> other;
    };

public:
    static T* allocate();
    static T* allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);
    template <class... Args>
    static void construct(T* ptr, Args&&... args);

    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};

template <class T>
T* Allocator<T>::allocate() {
    T* tmp = static_cast<T*>(::operator new(sizeof(T)));
    if (tmp == 0) {
        // std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <class T>
T* Allocator<T>::allocate(size_type n) {
    if (n == 0) return nullptr;
    T* tmp = static_cast<T*>(::operator new(n * sizeof(T)));
    if (tmp == 0) {
        // std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <class T>
void Allocator<T>::deallocate(T* ptr) {
    if (ptr == nullptr) return;
    ::operator delete(ptr);
}

template <class T>
void Allocator<T>::deallocate(T* ptr, size_type /*size*/) {
    if (ptr == nullptr) return;
    ::operator delete(ptr);
}

template <class T>
void Allocator<T>::construct(T* ptr) {
    mySTL::construct(ptr);
}

template <class T>
void Allocator<T>::construct(T* ptr, const T& value) {
    mySTL::construct(ptr, value);
}

template <class T>
void Allocator<T>::construct(T* ptr, T&& value) {
    mySTL::construct(ptr, mySTL::move(value));
}

template <class T>
template <class... Args>
void Allocator<T>::construct(T* ptr, Args&&... args) {
    mySTL::construct(ptr, mySTL::forward<Args>(args)...);
}

template <class T>
void Allocator<T>::destroy(T* ptr) {
    mySTL::destroy(ptr);
}

template <class T>
void Allocator<T>::destroy(T* first, T* last) {
    mySTL::destroy(first, last);
}

}  // namespace mySTL
#endif  // !MYTINYSTL_ALLOCATOR_H_
