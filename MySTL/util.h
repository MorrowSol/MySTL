#ifndef _UTIL_H_
#define _UTIL_H_

namespace mySTL {

/// remove_reference
template <typename _Tp>
struct remove_reference {
    typedef _Tp type;
};

template <typename _Tp>
struct remove_reference<_Tp&> {
    typedef _Tp type;
};

template <typename _Tp>
struct remove_reference<_Tp&&> {
    typedef _Tp type;
};

// forward
// 左值 返回 左值
template <class T>
T&& forward(typename mySTL::remove_reference<T>::type& arg) {
    return static_cast<T&&>(arg);
}
// 右值 返回 右值
template <class T>
T&& forward(typename mySTL::remove_reference<T>::type&& arg) {
    return static_cast<T&&>(arg);
}

// move
template <class T>
typename mySTL::remove_reference<T>::type&& move(T&& arg) {
    return static_cast<typename mySTL::remove_reference<T>::type&&>(arg);
}

template <class T>
T max(const T& a, const T& b) {
    return a > b ? a : b;
}
template <class T>
T min(const T& a, const T& b) {
    return a < b ? a : b;
}

// pair
template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    // 默认构造函数
    pair() : first(T1()), second(T2()) {}

    // 构造函数
    pair(const T1& a, const T2& b) : first(a), second(b) {}

    // 拷贝构造函数
    template <class U, class V>
    pair(const pair<U, V>& p) : first(p.first), second(p.second) {}
};

// 比较函数
template <class T1, class T2>
bool operator==(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
    return p1.first == p2.first && p1.second == p2.second;
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
    return (p1.first > p2.first) ||
           (!(p1.first < p2.first) && p1.second > p2.second);
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& s1, const pair<T1, T2>& s2) {
    return (s1.first < s2.first) ||
           (!(s1.first > s2.first) && s1.second < s2.second);
}
// make_pair
template <typename T1, typename T2>
pair<T1, T2> make_pair(const T1& f, const T2& s) {
    return pair<T1, T2>(f, s);
}
}  // namespace mySTL
#endif