#ifndef _UTIL_H_
#define _UTIL_H_

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

template <class T>
T max(const T& a, const T& b){
    return a>b?a:b;
}
template <class T>
T min(const T& a, const T& b){
    return a<b?a:b;
}

}
#endif