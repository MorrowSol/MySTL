#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

// 废弃
namespace mySTL {

// // 常量类型
// template <typename Tp, Tp _v>
// struct integral_constant {
//     static constexpr Tp value = __v;
//     typedef Tp value_type;
//     typedef integral_constant<Tp, _v> type;
// };
// typedef integral_constant<bool, true> true_type;
// typedef integral_constant<bool, false> false_type;

// template <bool __v>
// using __bool_constant = integral_constant<bool, __v>;

// // Primary template.
// /// Define a member typedef @c type to one of two argument types.
// template <bool _Cond, typename _Iftrue, typename _Iffalse>
// struct conditional {
//     typedef _Iftrue type;
// };
// // Partial specialization for false.
// template <typename _Iftrue, typename _Iffalse>
// struct conditional<false, _Iftrue, _Iffalse> {
//     typedef _Iffalse type;
// };
// template <typename _B1, typename _B2>
// struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type {};
// /// is_trivially_destructible
// template <typename _Tp>
// struct is_trivially_destructible
//     : public __and_<is_destructible<_Tp>,
//                     __bool_constant<__has_trivial_destructor(_Tp)>> {};

// }  // namespace mySTL

#endif