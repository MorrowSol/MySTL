#ifndef _UTIL_H_
#define _UTIL_H_

// size_t ptrdiff_t
#include <cstddef>

namespace mySTL{

/// remove_reference
template<typename _Tp>
struct remove_reference
{ typedef _Tp   type; };

template<typename _Tp>
struct remove_reference<_Tp&>
{ typedef _Tp   type; };

template<typename _Tp>
struct remove_reference<_Tp&&>
{ typedef _Tp   type; };


// forward
// 左值 返回 左值
template <class T>
T&& forward(typename mySTL::remove_reference<T>::type& arg){
    return static_cast<T&&>(arg);
}
// 右值 返回 右值
template <class T>
T&& forward(typename mySTL::remove_reference<T>::type&& arg){
    return static_cast<T&&>(arg);
}

// move
template <class T>
typename mySTL::remove_reference<T>::type&& move(T&& arg){
    return static_cast<typename mySTL::remove_reference<T>::type&&>(arg);
}


}
#endif