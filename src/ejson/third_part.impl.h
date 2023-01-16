#pragma once

#include <cstddef>
#include <exception>
#include <functional>
#include <initializer_list>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>

// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://boost.org/LICENSE_1_0.txt)

#ifndef MPARK_CONFIG_HPP
#define MPARK_CONFIG_HPP

// MSVC 2015 Update 3.
#if __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_FULL_VER < 190024210)
#error "MPark.Variant requires C++11 support."
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __has_include
#define __has_include(x) 0
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#if __has_attribute(always_inline) || defined(__GNUC__)
#define MPARK_ALWAYS_INLINE __attribute__((__always_inline__)) inline
#elif defined(_MSC_VER)
#define MPARK_ALWAYS_INLINE __forceinline
#else
#define MPARK_ALWAYS_INLINE inline
#endif

#if __has_builtin(__builtin_addressof) ||                                      \
    (defined(__GNUC__) && __GNUC__ >= 7) || defined(_MSC_VER)
#define MPARK_BUILTIN_ADDRESSOF
#endif

#if __has_builtin(__builtin_unreachable) || defined(__GNUC__)
#define MPARK_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define MPARK_BUILTIN_UNREACHABLE __assume(false)
#else
#define MPARK_BUILTIN_UNREACHABLE
#endif

#if __has_builtin(__type_pack_element)
#define MPARK_TYPE_PACK_ELEMENT
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 200704 &&                   \
    !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ == 9)
#define MPARK_CPP11_CONSTEXPR
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 201304
#define MPARK_CPP14_CONSTEXPR
#endif

#if __has_feature(cxx_exceptions) || defined(__cpp_exceptions) ||              \
    (defined(_MSC_VER) && defined(_CPPUNWIND))
#define MPARK_EXCEPTIONS
#endif

#if defined(__cpp_generic_lambdas) || defined(_MSC_VER)
#define MPARK_GENERIC_LAMBDAS
#endif

#if defined(__cpp_lib_integer_sequence)
#define MPARK_INTEGER_SEQUENCE
#endif

#if (defined(__cpp_decltype_auto) && defined(__cpp_return_type_deduction)) ||  \
    defined(_MSC_VER)
#define MPARK_RETURN_TYPE_DEDUCTION
#endif

#if defined(__cpp_lib_transparent_operators) || defined(_MSC_VER)
#define MPARK_TRANSPARENT_OPERATORS
#endif

#if defined(__cpp_variable_templates) || defined(_MSC_VER)
#define MPARK_VARIABLE_TEMPLATES
#endif

#if !defined(__GLIBCXX__) || __has_include(<codecvt>) // >= libstdc++-5
#define MPARK_TRIVIALITY_TYPE_TRAITS
#define MPARK_INCOMPLETE_TYPE_TRAITS
#endif

#endif // MPARK_CONFIG_HPP

// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://boost.org/LICENSE_1_0.txt)

#ifndef MPARK_IN_PLACE_HPP
#define MPARK_IN_PLACE_HPP

#include <cstddef>

namespace mpark {

struct in_place_t {
  explicit in_place_t() = default;
};

template <std::size_t I> struct in_place_index_t {
  explicit in_place_index_t() = default;
};

template <typename T> struct in_place_type_t {
  explicit in_place_type_t() = default;
};

#ifdef MPARK_VARIABLE_TEMPLATES
constexpr in_place_t in_place{};

template <std::size_t I> constexpr in_place_index_t<I> in_place_index{};

template <typename T> constexpr in_place_type_t<T> in_place_type{};
#endif

} // namespace mpark

#endif // MPARK_IN_PLACE_HPP

// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://boost.org/LICENSE_1_0.txt)

#ifndef MPARK_LIB_HPP
#define MPARK_LIB_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#define MPARK_RETURN(...)                                                      \
  noexcept(noexcept(__VA_ARGS__))->decltype(__VA_ARGS__) { return __VA_ARGS__; }

namespace mpark {
namespace lib {
template <typename T> struct identity {
  using type = T;
};

inline namespace cpp14 {
template <typename T, std::size_t N> struct array {
  constexpr const T &operator[](std::size_t index) const { return data[index]; }

  T data[N == 0 ? 1 : N];
};

template <typename T> using add_pointer_t = typename std::add_pointer<T>::type;

template <typename... Ts>
using common_type_t = typename std::common_type<Ts...>::type;

template <typename T> using decay_t = typename std::decay<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
using remove_cvref_t = typename std::remove_cv<remove_reference_t<T>>::type;

template <typename T>
inline constexpr T &&forward(remove_reference_t<T> &t) noexcept {
  return static_cast<T &&>(t);
}

template <typename T>
inline constexpr T &&forward(remove_reference_t<T> &&t) noexcept {
  static_assert(!std::is_lvalue_reference<T>::value,
                "can not forward an rvalue as an lvalue");
  return static_cast<T &&>(t);
}

template <typename T>
inline constexpr remove_reference_t<T> &&move(T &&t) noexcept {
  return static_cast<remove_reference_t<T> &&>(t);
}

#ifdef MPARK_INTEGER_SEQUENCE
using std::index_sequence;
using std::index_sequence_for;
using std::integer_sequence;
using std::make_index_sequence;
#else
template <typename T, T... Is> struct integer_sequence {
  using value_type = T;
  static constexpr std::size_t size() noexcept { return sizeof...(Is); }
};

template <std::size_t... Is>
using index_sequence = integer_sequence<std::size_t, Is...>;

template <typename Lhs, typename Rhs> struct make_index_sequence_concat;

template <std::size_t... Lhs, std::size_t... Rhs>
struct make_index_sequence_concat<index_sequence<Lhs...>,
                                  index_sequence<Rhs...>>
    : identity<index_sequence<Lhs..., (sizeof...(Lhs) + Rhs)...>> {};

template <std::size_t N> struct make_index_sequence_impl;

template <std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

template <std::size_t N>
struct make_index_sequence_impl
    : make_index_sequence_concat<make_index_sequence<N / 2>,
                                 make_index_sequence<N - (N / 2)>> {};

template <> struct make_index_sequence_impl<0> : identity<index_sequence<>> {};

template <> struct make_index_sequence_impl<1> : identity<index_sequence<0>> {};

template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;
#endif

// <functional>
#ifdef MPARK_TRANSPARENT_OPERATORS
using equal_to = std::equal_to<>;
#else
struct equal_to {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) == lib::forward<Rhs>(rhs))
};
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
using not_equal_to = std::not_equal_to<>;
#else
struct not_equal_to {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) != lib::forward<Rhs>(rhs))
};
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
using less = std::less<>;
#else
struct less {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) < lib::forward<Rhs>(rhs))
};
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
using greater = std::greater<>;
#else
struct greater {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) > lib::forward<Rhs>(rhs))
};
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
using less_equal = std::less_equal<>;
#else
struct less_equal {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) <= lib::forward<Rhs>(rhs))
};
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
using greater_equal = std::greater_equal<>;
#else
struct greater_equal {
  template <typename Lhs, typename Rhs>
  inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
      MPARK_RETURN(lib::forward<Lhs>(lhs) >= lib::forward<Rhs>(rhs))
};
#endif
} // namespace cpp14

inline namespace cpp17 {

// <type_traits>
template <bool B> using bool_constant = std::integral_constant<bool, B>;

template <typename...> struct voider : identity<void> {};

template <typename... Ts> using void_t = typename voider<Ts...>::type;

namespace detail {
namespace swappable {

using std::swap;

template <typename T> struct is_swappable {
private:
  template <typename U,
            typename = decltype(swap(std::declval<U &>(), std::declval<U &>()))>
  inline static std::true_type test(int);

  template <typename U> inline static std::false_type test(...);

public:
  static constexpr bool value = decltype(test<T>(0))::value;
};

template <bool IsSwappable, typename T> struct is_nothrow_swappable {
  static constexpr bool value =
      noexcept(swap(std::declval<T &>(), std::declval<T &>()));
};

template <typename T>
struct is_nothrow_swappable<false, T> : std::false_type {};

} // namespace swappable
} // namespace detail

using detail::swappable::is_swappable;

template <typename T>
using is_nothrow_swappable =
    detail::swappable::is_nothrow_swappable<is_swappable<T>::value, T>;

// <functional>
namespace detail {

template <typename T> struct is_reference_wrapper : std::false_type {};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <bool, int> struct Invoke;

template <> struct Invoke<true /* pmf */, 0 /* is_base_of */> {
  template <typename R, typename T, typename Arg, typename... Args>
  inline static constexpr auto invoke(R T::*pmf, Arg &&arg, Args &&...args)
      MPARK_RETURN((lib::forward<Arg>(arg).*pmf)(lib::forward<Args>(args)...))
};

template <> struct Invoke<true /* pmf */, 1 /* is_reference_wrapper */> {
  template <typename R, typename T, typename Arg, typename... Args>
  inline static constexpr auto invoke(R T::*pmf, Arg &&arg, Args &&...args)
      MPARK_RETURN((lib::forward<Arg>(arg).get().*
                    pmf)(lib::forward<Args>(args)...))
};

template <> struct Invoke<true /* pmf */, 2 /* otherwise */> {
  template <typename R, typename T, typename Arg, typename... Args>
  inline static constexpr auto invoke(R T::*pmf, Arg &&arg, Args &&...args)
      MPARK_RETURN(((*lib::forward<Arg>(arg)).*
                    pmf)(lib::forward<Args>(args)...))
};

template <> struct Invoke<false /* pmo */, 0 /* is_base_of */> {
  template <typename R, typename T, typename Arg>
  inline static constexpr auto invoke(R T::*pmo, Arg &&arg)
      MPARK_RETURN(lib::forward<Arg>(arg).*pmo)
};

template <> struct Invoke<false /* pmo */, 1 /* is_reference_wrapper */> {
  template <typename R, typename T, typename Arg>
  inline static constexpr auto invoke(R T::*pmo, Arg &&arg)
      MPARK_RETURN(lib::forward<Arg>(arg).get().*pmo)
};

template <> struct Invoke<false /* pmo */, 2 /* otherwise */> {
  template <typename R, typename T, typename Arg>
  inline static constexpr auto invoke(R T::*pmo, Arg &&arg)
      MPARK_RETURN((*lib::forward<Arg>(arg)).*pmo)
};

template <typename R, typename T, typename Arg, typename... Args>
inline constexpr auto invoke(R T::*f, Arg &&arg, Args &&...args)
    MPARK_RETURN(Invoke<std::is_function<R>::value,
                        (std::is_base_of<T, lib::decay_t<Arg>>::value ? 0
                         : is_reference_wrapper<lib::decay_t<Arg>>::value
                             ? 1
                             : 2)>::invoke(f, lib::forward<Arg>(arg),
                                           lib::forward<Args>(args)...))

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#endif
        template <typename F, typename... Args>
        inline constexpr auto invoke(F &&f, Args &&...args)
            MPARK_RETURN(lib::forward<F>(f)(lib::forward<Args>(args)...))
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} // namespace detail

template <typename F, typename... Args>
inline constexpr auto invoke(F &&f, Args &&...args)
    MPARK_RETURN(detail::invoke(lib::forward<F>(f),
                                lib::forward<Args>(args)...))

        namespace detail {

  template <typename Void, typename, typename...> struct invoke_result {};

  template <typename F, typename... Args>
  struct invoke_result<
      void_t<decltype(lib::invoke(std::declval<F>(), std::declval<Args>()...))>,
      F, Args...> : identity<decltype(lib::invoke(std::declval<F>(),
                                                  std::declval<Args>()...))> {};

} // namespace detail

template <typename F, typename... Args>
using invoke_result = detail::invoke_result<void, F, Args...>;

template <typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

namespace detail {

template <typename Void, typename, typename...>
struct is_invocable : std::false_type {};

template <typename F, typename... Args>
struct is_invocable<void_t<invoke_result_t<F, Args...>>, F, Args...>
    : std::true_type {};

template <typename Void, typename, typename, typename...>
struct is_invocable_r : std::false_type {};

template <typename R, typename F, typename... Args>
struct is_invocable_r<void_t<invoke_result_t<F, Args...>>, R, F, Args...>
    : std::is_convertible<invoke_result_t<F, Args...>, R> {};

} // namespace detail

template <typename F, typename... Args>
using is_invocable = detail::is_invocable<void, F, Args...>;

template <typename R, typename F, typename... Args>
using is_invocable_r = detail::is_invocable_r<void, R, F, Args...>;

// <memory>
#ifdef MPARK_BUILTIN_ADDRESSOF
template <typename T> inline constexpr T *addressof(T &arg) noexcept {
  return __builtin_addressof(arg);
}
#else
namespace detail {

namespace has_addressof_impl {

struct fail;

template <typename T> inline fail operator&(T &&);

template <typename T> inline static constexpr bool impl() {
  return (std::is_class<T>::value || std::is_union<T>::value) &&
         !std::is_same<decltype(&std::declval<T &>()), fail>::value;
}

} // namespace has_addressof_impl

template <typename T>
using has_addressof = bool_constant<has_addressof_impl::impl<T>()>;

template <typename T>
inline constexpr T *addressof(T &arg, std::true_type) noexcept {
  return std::addressof(arg);
}

template <typename T>
inline constexpr T *addressof(T &arg, std::false_type) noexcept {
  return &arg;
}

} // namespace detail

template <typename T> inline constexpr T *addressof(T &arg) noexcept {
  return detail::addressof(arg, detail::has_addressof<T>{});
}
#endif

template <typename T> inline constexpr T *addressof(const T &&) = delete;

} // namespace cpp17

template <typename T> struct remove_all_extents : identity<T> {};

template <typename T, std::size_t N>
struct remove_all_extents<array<T, N>> : remove_all_extents<T> {};

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <std::size_t N>
using size_constant = std::integral_constant<std::size_t, N>;

template <std::size_t I, typename T> struct indexed_type : size_constant<I> {
  using type = T;
};

template <bool... Bs>
using all = std::is_same<integer_sequence<bool, true, Bs...>,
                         integer_sequence<bool, Bs..., true>>;

#ifdef MPARK_TYPE_PACK_ELEMENT
template <std::size_t I, typename... Ts>
using type_pack_element_t = __type_pack_element<I, Ts...>;
#else
template <std::size_t I, typename... Ts> struct type_pack_element_impl {
private:
  template <typename> struct set;

  template <std::size_t... Is>
  struct set<index_sequence<Is...>> : indexed_type<Is, Ts>... {};

  template <typename T>
  inline static std::enable_if<true, T> impl(indexed_type<I, T>);

  inline static std::enable_if<false> impl(...);

public:
  using type = decltype(impl(set<index_sequence_for<Ts...>>{}));
};

template <std::size_t I, typename... Ts>
using type_pack_element = typename type_pack_element_impl<I, Ts...>::type;

template <std::size_t I, typename... Ts>
using type_pack_element_t = typename type_pack_element<I, Ts...>::type;
#endif

#ifdef MPARK_TRIVIALITY_TYPE_TRAITS
using std::is_trivially_copy_assignable;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_assignable;
using std::is_trivially_move_constructible;
#else
template <typename T>
struct is_trivially_copy_constructible
    : bool_constant<std::is_copy_constructible<T>::value &&__has_trivial_copy(
          T)> {};

template <typename T>
struct is_trivially_move_constructible : bool_constant<__is_trivial(T)> {};

template <typename T>
struct is_trivially_copy_assignable
    : bool_constant<std::is_copy_assignable<T>::value &&__has_trivial_assign(
          T)> {};

template <typename T>
struct is_trivially_move_assignable : bool_constant<__is_trivial(T)> {};
#endif

template <typename T, bool> struct dependent_type : T {};

template <typename Is, std::size_t J> struct push_back;

template <typename Is, std::size_t J>
using push_back_t = typename push_back<Is, J>::type;

template <std::size_t... Is, std::size_t J>
struct push_back<index_sequence<Is...>, J> {
  using type = index_sequence<Is..., J>;
};

} // namespace lib
} // namespace mpark

#undef MPARK_RETURN

#endif // MPARK_LIB_HPP

namespace mpark {

#ifdef MPARK_RETURN_TYPE_DEDUCTION

#define AUTO auto
#define AUTO_RETURN(...)                                                       \
  { return __VA_ARGS__; }

#define AUTO_REFREF auto &&
#define AUTO_REFREF_RETURN(...)                                                \
  { return __VA_ARGS__; }

#define DECLTYPE_AUTO decltype(auto)
#define DECLTYPE_AUTO_RETURN(...)                                              \
  { return __VA_ARGS__; }

#else

#define AUTO auto
#define AUTO_RETURN(...)                                                       \
  ->lib::decay_t<decltype(__VA_ARGS__)> { return __VA_ARGS__; }

#define AUTO_REFREF auto
#define AUTO_REFREF_RETURN(...)                                                \
  ->decltype((__VA_ARGS__)) {                                                  \
    static_assert(std::is_reference<decltype((__VA_ARGS__))>::value, "");      \
    return __VA_ARGS__;                                                        \
  }

#define DECLTYPE_AUTO auto
#define DECLTYPE_AUTO_RETURN(...)                                              \
  ->decltype(__VA_ARGS__) { return __VA_ARGS__; }

#endif

class bad_variant_access : public std::exception {
public:
   const char *what() const noexcept override { return "bad_variant_access"; }
};

[[noreturn]] inline void throw_bad_variant_access() {
#ifdef MPARK_EXCEPTIONS
  throw bad_variant_access{};
#else
  std::terminate();
  MPARK_BUILTIN_UNREACHABLE;
#endif
}

template <typename... Ts> class variant;

template <typename T> struct variant_size;

#ifdef MPARK_VARIABLE_TEMPLATES
template <typename T>
constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

template <typename T> struct variant_size<const T> : variant_size<T> {};

template <typename T> struct variant_size<volatile T> : variant_size<T> {};

template <typename T>
struct variant_size<const volatile T> : variant_size<T> {};

template <typename... Ts>
struct variant_size<variant<Ts...>> : lib::size_constant<sizeof...(Ts)> {};

template <std::size_t I, typename T> struct variant_alternative;

template <std::size_t I, typename T>
using variant_alternative_t = typename variant_alternative<I, T>::type;

template <std::size_t I, typename T>
struct variant_alternative<I, const T>
    : std::add_const<variant_alternative_t<I, T>> {};

template <std::size_t I, typename T>
struct variant_alternative<I, volatile T>
    : std::add_volatile<variant_alternative_t<I, T>> {};

template <std::size_t I, typename T>
struct variant_alternative<I, const volatile T>
    : std::add_cv<variant_alternative_t<I, T>> {};

template <std::size_t I, typename... Ts>
struct variant_alternative<I, variant<Ts...>> {
  static_assert(I < sizeof...(Ts),
                "index out of bounds in `std::variant_alternative<>`");
  using type = lib::type_pack_element_t<I, Ts...>;
};

constexpr std::size_t variant_npos = static_cast<std::size_t>(-1);

namespace detail {

constexpr std::size_t not_found = static_cast<std::size_t>(-1);
constexpr std::size_t ambiguous = static_cast<std::size_t>(-2);

#ifdef MPARK_CPP14_CONSTEXPR
template <typename T, typename... Ts>
inline constexpr std::size_t find_index() {
  constexpr lib::array<bool, sizeof...(Ts)> matches = {
      {std::is_same<T, Ts>::value...}};
  std::size_t result = not_found;
  for (std::size_t i = 0; i < sizeof...(Ts); ++i) {
    if (matches[i]) {
      if (result != not_found) {
        return ambiguous;
      }
      result = i;
    }
  }
  return result;
}
#else
inline constexpr std::size_t find_index_impl(std::size_t result, std::size_t) {
  return result;
}

template <typename... Bs>
inline constexpr std::size_t
find_index_impl(std::size_t result, std::size_t idx, bool b, Bs... bs) {
  return b ? (result != not_found ? ambiguous
                                  : find_index_impl(idx, idx + 1, bs...))
           : find_index_impl(result, idx + 1, bs...);
}

template <typename T, typename... Ts>
inline constexpr std::size_t find_index() {
  return find_index_impl(not_found, 0, std::is_same<T, Ts>::value...);
}
#endif

template <std::size_t I>
using find_index_sfinae_impl =
    lib::enable_if_t<I != not_found && I != ambiguous, lib::size_constant<I>>;

template <typename T, typename... Ts>
using find_index_sfinae = find_index_sfinae_impl<find_index<T, Ts...>()>;

template <std::size_t I>
struct find_index_checked_impl : lib::size_constant<I> {
  static_assert(I != not_found, "the specified type is not found.");
  static_assert(I != ambiguous, "the specified type is ambiguous.");
};

template <typename T, typename... Ts>
using find_index_checked = find_index_checked_impl<find_index<T, Ts...>()>;

struct valueless_t {};

enum class Trait { TriviallyAvailable, Available, Unavailable };

template <typename T, template <typename> class IsTriviallyAvailable,
          template <typename> class IsAvailable>
inline constexpr Trait trait() {
  return IsTriviallyAvailable<T>::value ? Trait::TriviallyAvailable
         : IsAvailable<T>::value        ? Trait::Available
                                        : Trait::Unavailable;
}

#ifdef MPARK_CPP14_CONSTEXPR
template <typename... Traits>
inline constexpr Trait common_trait(Traits... traits_) {
  Trait result = Trait::TriviallyAvailable;
  lib::array<Trait, sizeof...(Traits)> traits = {{traits_...}};
  for (std::size_t i = 0; i < sizeof...(Traits); ++i) {
    Trait t = traits[i];
    if (static_cast<int>(t) > static_cast<int>(result)) {
      result = t;
    }
  }
  return result;
}
#else
inline constexpr Trait common_trait_impl(Trait result) { return result; }

template <typename... Traits>
inline constexpr Trait common_trait_impl(Trait result, Trait t, Traits... ts) {
  return static_cast<int>(t) > static_cast<int>(result)
             ? common_trait_impl(t, ts...)
             : common_trait_impl(result, ts...);
}

template <typename... Traits>
inline constexpr Trait common_trait(Traits... ts) {
  return common_trait_impl(Trait::TriviallyAvailable, ts...);
}
#endif

template <typename... Ts> struct traits {
  static constexpr Trait copy_constructible_trait =
      common_trait(trait<Ts, lib::is_trivially_copy_constructible,
                         std::is_copy_constructible>()...);

  static constexpr Trait move_constructible_trait =
      common_trait(trait<Ts, lib::is_trivially_move_constructible,
                         std::is_move_constructible>()...);

  static constexpr Trait copy_assignable_trait = common_trait(
      copy_constructible_trait, trait<Ts, lib::is_trivially_copy_assignable,
                                      std::is_copy_assignable>()...);

  static constexpr Trait move_assignable_trait = common_trait(
      move_constructible_trait, trait<Ts, lib::is_trivially_move_assignable,
                                      std::is_move_assignable>()...);

  static constexpr Trait destructible_trait = common_trait(
      trait<Ts, std::is_trivially_destructible, std::is_destructible>()...);
};

namespace access {

struct recursive_union {
#ifdef MPARK_RETURN_TYPE_DEDUCTION
  template <typename V>
  inline static constexpr auto &&get_alt(V &&v, in_place_index_t<0>) {
    return lib::forward<V>(v).head_;
  }

  template <typename V, std::size_t I>
  inline static constexpr auto &&get_alt(V &&v, in_place_index_t<I>) {
    return get_alt(lib::forward<V>(v).tail_, in_place_index_t<I - 1>{});
  }
#else
  template <std::size_t I, bool Dummy = true> struct get_alt_impl {
    template <typename V>
    inline constexpr AUTO_REFREF operator()(V &&v) const
        AUTO_REFREF_RETURN(get_alt_impl<I - 1>{}(lib::forward<V>(v).tail_))
  };

  template <bool Dummy> struct get_alt_impl<0, Dummy> {
    template <typename V>
    inline constexpr AUTO_REFREF operator()(V &&v) const
        AUTO_REFREF_RETURN(lib::forward<V>(v).head_)
  };

  template <typename V, std::size_t I>
  inline static constexpr AUTO_REFREF get_alt(V &&v, in_place_index_t<I>)
      AUTO_REFREF_RETURN(get_alt_impl<I>{}(lib::forward<V>(v)))
#endif
};

struct base {
  template <std::size_t I, typename V>
  inline static constexpr AUTO_REFREF get_alt(V &&v)
#ifdef _MSC_VER
      AUTO_REFREF_RETURN(recursive_union::get_alt(lib::forward<V>(v).data_,
                                                  in_place_index_t<I>{}))
#else
      AUTO_REFREF_RETURN(recursive_union::get_alt(data(lib::forward<V>(v)),
                                                  in_place_index_t<I>{}))
#endif
};

struct variant {
  template <std::size_t I, typename V>
  inline static constexpr AUTO_REFREF get_alt(V &&v)
      AUTO_REFREF_RETURN(base::get_alt<I>(lib::forward<V>(v).impl_))
};

} // namespace access

namespace visitation {

#if defined(MPARK_CPP14_CONSTEXPR) && !defined(_MSC_VER)
#define MPARK_VARIANT_SWITCH_VISIT
#endif

struct base {
  template <typename Visitor, typename... Vs>
  using dispatch_result_t =
      decltype(lib::invoke(std::declval<Visitor>(),
                           access::base::get_alt<0>(std::declval<Vs>())...));

  template <typename Expected> struct expected {
    template <typename Actual> inline static constexpr bool but_got() {
      return std::is_same<Expected, Actual>::value;
    }
  };

  template <typename Expected, typename Actual> struct visit_return_type_check {
    static_assert(expected<Expected>::template but_got<Actual>(),
                  "`visit` requires the visitor to have a single return type");

    template <typename Visitor, typename... Alts>
    inline static constexpr DECLTYPE_AUTO invoke(Visitor &&visitor,
                                                 Alts &&...alts)
        DECLTYPE_AUTO_RETURN(lib::invoke(lib::forward<Visitor>(visitor),
                                         lib::forward<Alts>(alts)...))
  };

#ifdef MPARK_VARIANT_SWITCH_VISIT
  template <bool B, typename R, typename... ITs> struct dispatcher;

  template <typename R, typename... ITs> struct dispatcher<false, R, ITs...> {
    template <std::size_t B, typename F, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R
    dispatch(F &&, typename ITs::type &&..., Vs &&...) {
      MPARK_BUILTIN_UNREACHABLE;
    }

    template <std::size_t I, typename F, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R dispatch_case(F &&, Vs &&...) {
      MPARK_BUILTIN_UNREACHABLE;
    }

    template <std::size_t B, typename F, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R dispatch_at(std::size_t, F &&,
                                                       Vs &&...) {
      MPARK_BUILTIN_UNREACHABLE;
    }
  };

  template <typename R, typename... ITs> struct dispatcher<true, R, ITs...> {
    template <std::size_t B, typename F>
    MPARK_ALWAYS_INLINE static constexpr R
    dispatch(F &&f, typename ITs::type &&...visited_vs) {
      using Expected = R;
      using Actual = decltype(lib::invoke(
          lib::forward<F>(f),
          access::base::get_alt<ITs::value>(
              lib::forward<typename ITs::type>(visited_vs))...));
      return visit_return_type_check<Expected, Actual>::invoke(
          lib::forward<F>(f),
          access::base::get_alt<ITs::value>(
              lib::forward<typename ITs::type>(visited_vs))...);
    }

    template <std::size_t B, typename F, typename V, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R
    dispatch(F &&f, typename ITs::type &&...visited_vs, V &&v, Vs &&...vs) {
#define MPARK_DISPATCH(I)                                                      \
  dispatcher<(I < lib::decay_t<V>::size()), R, ITs...,                         \
             lib::indexed_type<I, V>>::                                        \
      template dispatch<0>(lib::forward<F>(f),                                 \
                           lib::forward<typename ITs::type>(visited_vs)...,    \
                           lib::forward<V>(v), lib::forward<Vs>(vs)...)

#define MPARK_DEFAULT(I)                                                       \
  dispatcher<(I < lib::decay_t<V>::size()), R, ITs...>::template dispatch<I>(  \
      lib::forward<F>(f), lib::forward<typename ITs::type>(visited_vs)...,     \
      lib::forward<V>(v), lib::forward<Vs>(vs)...)

      switch (v.index()) {
      case B + 0:
        return MPARK_DISPATCH(B + 0);
      case B + 1:
        return MPARK_DISPATCH(B + 1);
      case B + 2:
        return MPARK_DISPATCH(B + 2);
      case B + 3:
        return MPARK_DISPATCH(B + 3);
      case B + 4:
        return MPARK_DISPATCH(B + 4);
      case B + 5:
        return MPARK_DISPATCH(B + 5);
      case B + 6:
        return MPARK_DISPATCH(B + 6);
      case B + 7:
        return MPARK_DISPATCH(B + 7);
      case B + 8:
        return MPARK_DISPATCH(B + 8);
      case B + 9:
        return MPARK_DISPATCH(B + 9);
      case B + 10:
        return MPARK_DISPATCH(B + 10);
      case B + 11:
        return MPARK_DISPATCH(B + 11);
      case B + 12:
        return MPARK_DISPATCH(B + 12);
      case B + 13:
        return MPARK_DISPATCH(B + 13);
      case B + 14:
        return MPARK_DISPATCH(B + 14);
      case B + 15:
        return MPARK_DISPATCH(B + 15);
      case B + 16:
        return MPARK_DISPATCH(B + 16);
      case B + 17:
        return MPARK_DISPATCH(B + 17);
      case B + 18:
        return MPARK_DISPATCH(B + 18);
      case B + 19:
        return MPARK_DISPATCH(B + 19);
      case B + 20:
        return MPARK_DISPATCH(B + 20);
      case B + 21:
        return MPARK_DISPATCH(B + 21);
      case B + 22:
        return MPARK_DISPATCH(B + 22);
      case B + 23:
        return MPARK_DISPATCH(B + 23);
      case B + 24:
        return MPARK_DISPATCH(B + 24);
      case B + 25:
        return MPARK_DISPATCH(B + 25);
      case B + 26:
        return MPARK_DISPATCH(B + 26);
      case B + 27:
        return MPARK_DISPATCH(B + 27);
      case B + 28:
        return MPARK_DISPATCH(B + 28);
      case B + 29:
        return MPARK_DISPATCH(B + 29);
      case B + 30:
        return MPARK_DISPATCH(B + 30);
      case B + 31:
        return MPARK_DISPATCH(B + 31);
      default:
        return MPARK_DEFAULT(B + 32);
      }

#undef MPARK_DEFAULT
#undef MPARK_DISPATCH
    }

    template <std::size_t I, typename F, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R dispatch_case(F &&f, Vs &&...vs) {
      using Expected = R;
      using Actual = decltype(lib::invoke(
          lib::forward<F>(f),
          access::base::get_alt<I>(lib::forward<Vs>(vs))...));
      return visit_return_type_check<Expected, Actual>::invoke(
          lib::forward<F>(f),
          access::base::get_alt<I>(lib::forward<Vs>(vs))...);
    }

    template <std::size_t B, typename F, typename V, typename... Vs>
    MPARK_ALWAYS_INLINE static constexpr R dispatch_at(std::size_t index, F &&f,
                                                       V &&v, Vs &&...vs) {
      static_assert(lib::all<(lib::decay_t<V>::size() ==
                              lib::decay_t<Vs>::size())...>::value,
                    "all of the variants must be the same size.");
#define MPARK_DISPATCH_AT(I)                                                   \
  dispatcher<(I < lib::decay_t<V>::size()), R>::template dispatch_case<I>(     \
      lib::forward<F>(f), lib::forward<V>(v), lib::forward<Vs>(vs)...)

#define MPARK_DEFAULT(I)                                                       \
  dispatcher<(I < lib::decay_t<V>::size()), R>::template dispatch_at<I>(       \
      index, lib::forward<F>(f), lib::forward<V>(v), lib::forward<Vs>(vs)...)

      switch (index) {
      case B + 0:
        return MPARK_DISPATCH_AT(B + 0);
      case B + 1:
        return MPARK_DISPATCH_AT(B + 1);
      case B + 2:
        return MPARK_DISPATCH_AT(B + 2);
      case B + 3:
        return MPARK_DISPATCH_AT(B + 3);
      case B + 4:
        return MPARK_DISPATCH_AT(B + 4);
      case B + 5:
        return MPARK_DISPATCH_AT(B + 5);
      case B + 6:
        return MPARK_DISPATCH_AT(B + 6);
      case B + 7:
        return MPARK_DISPATCH_AT(B + 7);
      case B + 8:
        return MPARK_DISPATCH_AT(B + 8);
      case B + 9:
        return MPARK_DISPATCH_AT(B + 9);
      case B + 10:
        return MPARK_DISPATCH_AT(B + 10);
      case B + 11:
        return MPARK_DISPATCH_AT(B + 11);
      case B + 12:
        return MPARK_DISPATCH_AT(B + 12);
      case B + 13:
        return MPARK_DISPATCH_AT(B + 13);
      case B + 14:
        return MPARK_DISPATCH_AT(B + 14);
      case B + 15:
        return MPARK_DISPATCH_AT(B + 15);
      case B + 16:
        return MPARK_DISPATCH_AT(B + 16);
      case B + 17:
        return MPARK_DISPATCH_AT(B + 17);
      case B + 18:
        return MPARK_DISPATCH_AT(B + 18);
      case B + 19:
        return MPARK_DISPATCH_AT(B + 19);
      case B + 20:
        return MPARK_DISPATCH_AT(B + 20);
      case B + 21:
        return MPARK_DISPATCH_AT(B + 21);
      case B + 22:
        return MPARK_DISPATCH_AT(B + 22);
      case B + 23:
        return MPARK_DISPATCH_AT(B + 23);
      case B + 24:
        return MPARK_DISPATCH_AT(B + 24);
      case B + 25:
        return MPARK_DISPATCH_AT(B + 25);
      case B + 26:
        return MPARK_DISPATCH_AT(B + 26);
      case B + 27:
        return MPARK_DISPATCH_AT(B + 27);
      case B + 28:
        return MPARK_DISPATCH_AT(B + 28);
      case B + 29:
        return MPARK_DISPATCH_AT(B + 29);
      case B + 30:
        return MPARK_DISPATCH_AT(B + 30);
      case B + 31:
        return MPARK_DISPATCH_AT(B + 31);
      default:
        return MPARK_DEFAULT(B + 32);
      }

#undef MPARK_DEFAULT
#undef MPARK_DISPATCH_AT
    }
  };
#else
  template <typename T>
  inline static constexpr const T &at(const T &elem) noexcept {
    return elem;
  }

  template <typename T, std::size_t N, typename... Is>
  inline static constexpr const lib::remove_all_extents_t<T> &
  at(const lib::array<T, N> &elems, std::size_t i, Is... is) noexcept {
    return at(elems[i], is...);
  }

  template <typename F, typename... Fs>
  inline static constexpr lib::array<lib::decay_t<F>, sizeof...(Fs) + 1>
  make_farray(F &&f, Fs &&...fs) {
    return {{lib::forward<F>(f), lib::forward<Fs>(fs)...}};
  }

  template <typename F, typename... Vs> struct make_fmatrix_impl {

    template <std::size_t... Is>
    inline static constexpr dispatch_result_t<F, Vs...> dispatch(F &&f,
                                                                 Vs &&...vs) {
      using Expected = dispatch_result_t<F, Vs...>;
      using Actual = decltype(lib::invoke(
          lib::forward<F>(f),
          access::base::get_alt<Is>(lib::forward<Vs>(vs))...));
      return visit_return_type_check<Expected, Actual>::invoke(
          lib::forward<F>(f),
          access::base::get_alt<Is>(lib::forward<Vs>(vs))...);
    }

#ifdef MPARK_RETURN_TYPE_DEDUCTION
    template <std::size_t... Is>
    inline static constexpr auto impl(lib::index_sequence<Is...>) {
      return &dispatch<Is...>;
    }

    template <typename Is, std::size_t... Js, typename... Ls>
    inline static constexpr auto impl(Is, lib::index_sequence<Js...>,
                                      Ls... ls) {
      return make_farray(impl(lib::push_back_t<Is, Js>{}, ls...)...);
    }
#else
    template <typename...> struct impl;

    template <std::size_t... Is> struct impl<lib::index_sequence<Is...>> {
      inline constexpr AUTO operator()() const AUTO_RETURN(&dispatch<Is...>)
    };

    template <typename Is, std::size_t... Js, typename... Ls>
    struct impl<Is, lib::index_sequence<Js...>, Ls...> {
      inline constexpr AUTO operator()() const
          AUTO_RETURN(make_farray(impl<lib::push_back_t<Is, Js>, Ls...>{}()...))
    };
#endif
  };

#ifdef MPARK_RETURN_TYPE_DEDUCTION
  template <typename F, typename... Vs>
  inline static constexpr auto make_fmatrix() {
    return make_fmatrix_impl<F, Vs...>::impl(
        lib::index_sequence<>{},
        lib::make_index_sequence<lib::decay_t<Vs>::size()>{}...);
  }
#else
  template <typename F, typename... Vs>
  inline static constexpr AUTO make_fmatrix()
      AUTO_RETURN(
          typename make_fmatrix_impl<F, Vs...>::template impl<
              lib::index_sequence<>,
              lib::make_index_sequence<lib::decay_t<Vs>::size()>...>{}())
#endif

  template <typename F, typename... Vs> struct make_fdiagonal_impl {
    template <std::size_t I>
    inline static constexpr dispatch_result_t<F, Vs...> dispatch(F &&f,
                                                                 Vs &&...vs) {
      using Expected = dispatch_result_t<F, Vs...>;
      using Actual = decltype(lib::invoke(
          lib::forward<F>(f),
          access::base::get_alt<I>(lib::forward<Vs>(vs))...));
      return visit_return_type_check<Expected, Actual>::invoke(
          lib::forward<F>(f),
          access::base::get_alt<I>(lib::forward<Vs>(vs))...);
    }

    template <std::size_t... Is>
    inline static constexpr AUTO impl(lib::index_sequence<Is...>)
        AUTO_RETURN(make_farray(&dispatch<Is>...))
  };

  template <typename F, typename V, typename... Vs>
  inline static constexpr auto make_fdiagonal()
      -> decltype(make_fdiagonal_impl<F, V, Vs...>::impl(
          lib::make_index_sequence<lib::decay_t<V>::size()>{})) {
    static_assert(lib::all<(lib::decay_t<V>::size() ==
                            lib::decay_t<Vs>::size())...>::value,
                  "all of the variants must be the same size.");
    return make_fdiagonal_impl<F, V, Vs...>::impl(
        lib::make_index_sequence<lib::decay_t<V>::size()>{});
  }
#endif
};

#if !defined(MPARK_VARIANT_SWITCH_VISIT) &&                                    \
    (!defined(_MSC_VER) || _MSC_VER >= 1910)
template <typename F, typename... Vs>
using fmatrix_t = decltype(base::make_fmatrix<F, Vs...>());

template <typename F, typename... Vs> struct fmatrix {
  static constexpr fmatrix_t<F, Vs...> value = base::make_fmatrix<F, Vs...>();
};

template <typename F, typename... Vs>
constexpr fmatrix_t<F, Vs...> fmatrix<F, Vs...>::value;

template <typename F, typename... Vs>
using fdiagonal_t = decltype(base::make_fdiagonal<F, Vs...>());

template <typename F, typename... Vs> struct fdiagonal {
  static constexpr fdiagonal_t<F, Vs...> value =
      base::make_fdiagonal<F, Vs...>();
};

template <typename F, typename... Vs>
constexpr fdiagonal_t<F, Vs...> fdiagonal<F, Vs...>::value;
#endif

struct alt {
  template <typename Visitor, typename... Vs>
  inline static constexpr DECLTYPE_AUTO visit_alt(Visitor &&visitor, Vs &&...vs)
#ifdef MPARK_VARIANT_SWITCH_VISIT
      DECLTYPE_AUTO_RETURN(
          base::dispatcher<
              true, base::dispatch_result_t<
                        Visitor, decltype(as_base(lib::forward<Vs>(vs)))...>>::
              template dispatch<0>(lib::forward<Visitor>(visitor),
                                   as_base(lib::forward<Vs>(vs))...))
#elif !defined(_MSC_VER) || _MSC_VER >= 1910
      DECLTYPE_AUTO_RETURN(
          base::at(fmatrix<Visitor &&,
                           decltype(as_base(lib::forward<Vs>(vs)))...>::value,
                   vs.index()...)(lib::forward<Visitor>(visitor),
                                  as_base(lib::forward<Vs>(vs))...))
#else
      DECLTYPE_AUTO_RETURN(base::at(
          base::make_fmatrix<Visitor &&,
                             decltype(as_base(lib::forward<Vs>(vs)))...>(),
          vs.index()...)(lib::forward<Visitor>(visitor),
                         as_base(lib::forward<Vs>(vs))...))
#endif

          template <typename Visitor, typename... Vs>
          inline static constexpr DECLTYPE_AUTO
      visit_alt_at(std::size_t index, Visitor &&visitor, Vs &&...vs)
#ifdef MPARK_VARIANT_SWITCH_VISIT
          DECLTYPE_AUTO_RETURN(
              base::dispatcher<
                  true,
                  base::dispatch_result_t<
                      Visitor, decltype(as_base(lib::forward<Vs>(vs)))...>>::
                  template dispatch_at<0>(index, lib::forward<Visitor>(visitor),
                                          as_base(lib::forward<Vs>(vs))...))
#elif !defined(_MSC_VER) || _MSC_VER >= 1910
          DECLTYPE_AUTO_RETURN(base::at(
              fdiagonal<Visitor &&,
                        decltype(as_base(lib::forward<Vs>(vs)))...>::value,
              index)(lib::forward<Visitor>(visitor),
                     as_base(lib::forward<Vs>(vs))...))
#else
          DECLTYPE_AUTO_RETURN(base::at(
              base::make_fdiagonal<
                  Visitor &&, decltype(as_base(lib::forward<Vs>(vs)))...>(),
              index)(lib::forward<Visitor>(visitor),
                     as_base(lib::forward<Vs>(vs))...))
#endif
};

struct variant {
private:
  template <typename Visitor> struct visitor {
    template <typename... Values>
    inline static constexpr bool does_not_handle() {
      return lib::is_invocable<Visitor, Values...>::value;
    }
  };

  template <typename Visitor, typename... Values>
  struct visit_exhaustiveness_check {
    static_assert(visitor<Visitor>::template does_not_handle<Values...>(),
                  "`visit` requires the visitor to be exhaustive.");

    inline static constexpr DECLTYPE_AUTO invoke(Visitor &&visitor,
                                                 Values &&...values)
        DECLTYPE_AUTO_RETURN(lib::invoke(lib::forward<Visitor>(visitor),
                                         lib::forward<Values>(values)...))
  };

  template <typename Visitor> struct value_visitor {
    Visitor &&visitor_;

    template <typename... Alts>
    inline constexpr DECLTYPE_AUTO operator()(Alts &&...alts) const
        DECLTYPE_AUTO_RETURN(
            visit_exhaustiveness_check<
                Visitor, decltype((lib::forward<Alts>(alts).value))...>::
                invoke(lib::forward<Visitor>(visitor_),
                       lib::forward<Alts>(alts).value...))
  };

  template <typename Visitor>
  inline static constexpr AUTO make_value_visitor(Visitor &&visitor)
      AUTO_RETURN(value_visitor<Visitor>{lib::forward<Visitor>(visitor)})

          public
      : template <typename Visitor, typename... Vs>
        inline static constexpr DECLTYPE_AUTO
        visit_alt(Visitor &&visitor, Vs &&...vs)
            DECLTYPE_AUTO_RETURN(alt::visit_alt(lib::forward<Visitor>(visitor),
                                                lib::forward<Vs>(vs).impl_...))

                template <typename Visitor, typename... Vs>
                inline static constexpr DECLTYPE_AUTO
        visit_alt_at(std::size_t index, Visitor &&visitor, Vs &&...vs)
            DECLTYPE_AUTO_RETURN(
                alt::visit_alt_at(index, lib::forward<Visitor>(visitor),
                                  lib::forward<Vs>(vs).impl_...))

                template <typename Visitor, typename... Vs>
                inline static constexpr DECLTYPE_AUTO
        visit_value(Visitor &&visitor, Vs &&...vs) DECLTYPE_AUTO_RETURN(
            visit_alt(make_value_visitor(lib::forward<Visitor>(visitor)),
                      lib::forward<Vs>(vs)...))

            template <typename Visitor, typename... Vs>
            inline static constexpr DECLTYPE_AUTO
        visit_value_at(std::size_t index, Visitor &&visitor, Vs &&...vs)
            DECLTYPE_AUTO_RETURN(visit_alt_at(
                index, make_value_visitor(lib::forward<Visitor>(visitor)),
                lib::forward<Vs>(vs)...))
};

} // namespace visitation

template <std::size_t Index, typename T> struct alt {
  using value_type = T;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
  template <typename... Args>
  inline explicit constexpr alt(in_place_t, Args &&...args)
      : value(lib::forward<Args>(args)...) {}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

  T value;
};

template <Trait DestructibleTrait, std::size_t Index, typename... Ts>
union recursive_union;

template <Trait DestructibleTrait, std::size_t Index>
union recursive_union<DestructibleTrait, Index> {};

#define MPARK_VARIANT_RECURSIVE_UNION(destructible_trait, destructor)          \
  template <std::size_t Index, typename T, typename... Ts>                     \
  union recursive_union<destructible_trait, Index, T, Ts...> {                 \
  public:                                                                      \
    inline explicit constexpr recursive_union(valueless_t) noexcept            \
        : dummy_{} {}                                                          \
                                                                               \
    template <typename... Args>                                                \
    inline explicit constexpr recursive_union(in_place_index_t<0>,             \
                                              Args &&...args)                  \
        : head_(in_place_t{}, lib::forward<Args>(args)...) {}                  \
                                                                               \
    template <std::size_t I, typename... Args>                                 \
    inline explicit constexpr recursive_union(in_place_index_t<I>,             \
                                              Args &&...args)                  \
        : tail_(in_place_index_t<I - 1>{}, lib::forward<Args>(args)...) {}     \
                                                                               \
    recursive_union(const recursive_union &) = default;                        \
    recursive_union(recursive_union &&) = default;                             \
                                                                               \
    destructor                                                                 \
                                                                               \
        recursive_union &                                                      \
        operator=(const recursive_union &) = default;                          \
    recursive_union &operator=(recursive_union &&) = default;                  \
                                                                               \
  private:                                                                     \
    char dummy_;                                                               \
    alt<Index, T> head_;                                                       \
    recursive_union<destructible_trait, Index + 1, Ts...> tail_;               \
                                                                               \
    friend struct access::recursive_union;                                     \
  }

MPARK_VARIANT_RECURSIVE_UNION(Trait::TriviallyAvailable,
                              ~recursive_union() = default;);
MPARK_VARIANT_RECURSIVE_UNION(Trait::Available, ~recursive_union(){});
MPARK_VARIANT_RECURSIVE_UNION(Trait::Unavailable, ~recursive_union() = delete;);

#undef MPARK_VARIANT_RECURSIVE_UNION

template <typename... Ts>
using index_t = typename std::conditional<
    sizeof...(Ts) < (std::numeric_limits<unsigned char>::max)(), unsigned char,
    typename std::conditional<sizeof...(Ts) <
                                  (std::numeric_limits<unsigned short>::max)(),
                              unsigned short, unsigned int>::type>::type;

template <Trait DestructibleTrait, typename... Ts> class base {
public:
  inline explicit constexpr base(valueless_t tag) noexcept
      : data_(tag), index_(static_cast<index_t<Ts...>>(-1)) {}

  template <std::size_t I, typename... Args>
  inline explicit constexpr base(in_place_index_t<I>, Args &&...args)
      : data_(in_place_index_t<I>{}, lib::forward<Args>(args)...), index_(I) {}

  inline constexpr bool valueless_by_exception() const noexcept {
    return index_ == static_cast<index_t<Ts...>>(-1);
  }

  inline constexpr std::size_t index() const noexcept {
    return valueless_by_exception() ? variant_npos : index_;
  }

protected:
  using data_t = recursive_union<DestructibleTrait, 0, Ts...>;

  friend inline constexpr base &as_base(base &b) { return b; }
  friend inline constexpr const base &as_base(const base &b) { return b; }
  friend inline constexpr base &&as_base(base &&b) { return lib::move(b); }
  friend inline constexpr const base &&as_base(const base &&b) {
    return lib::move(b);
  }

  friend inline constexpr data_t &data(base &b) { return b.data_; }
  friend inline constexpr const data_t &data(const base &b) { return b.data_; }
  friend inline constexpr data_t &&data(base &&b) { return lib::move(b).data_; }
  friend inline constexpr const data_t &&data(const base &&b) {
    return lib::move(b).data_;
  }

  inline static constexpr std::size_t size() { return sizeof...(Ts); }

  data_t data_;
  index_t<Ts...> index_;

  friend struct access::base;
  friend struct visitation::base;
};

struct dtor {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#endif
  template <typename Alt> inline void operator()(Alt &alt) const noexcept {
    alt.~Alt();
  }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
};

#if !defined(_MSC_VER) || _MSC_VER >= 1910
#define MPARK_INHERITING_CTOR(type, base) using base::base;
#else
#define MPARK_INHERITING_CTOR(type, base)                                      \
  template <typename... Args>                                                  \
  inline explicit constexpr type(Args &&...args)                               \
      : base(lib::forward<Args>(args)...) {}
#endif

template <typename Traits, Trait = Traits::destructible_trait> class destructor;

#define MPARK_VARIANT_DESTRUCTOR(destructible_trait, definition, destroy)      \
  template <typename... Ts>                                                    \
  class destructor<traits<Ts...>, destructible_trait>                          \
      : public base<destructible_trait, Ts...> {                               \
    using super = base<destructible_trait, Ts...>;                             \
                                                                               \
  public:                                                                      \
    MPARK_INHERITING_CTOR(destructor, super)                                   \
    using super::operator=;                                                    \
                                                                               \
    destructor(const destructor &) = default;                                  \
    destructor(destructor &&) = default;                                       \
    definition destructor &operator=(const destructor &) = default;            \
    destructor &operator=(destructor &&) = default;                            \
                                                                               \
  protected:                                                                   \
    destroy                                                                    \
  }

MPARK_VARIANT_DESTRUCTOR(
    Trait::TriviallyAvailable, ~destructor() = default;
    , inline void destroy() noexcept {
      this->index_ = static_cast<index_t<Ts...>>(-1);
    });

MPARK_VARIANT_DESTRUCTOR(
    Trait::Available, ~destructor() { destroy(); },
    inline void destroy() noexcept {
      if (!this->valueless_by_exception()) {
        visitation::alt::visit_alt(dtor{}, *this);
      }
      this->index_ = static_cast<index_t<Ts...>>(-1);
    });

MPARK_VARIANT_DESTRUCTOR(Trait::Unavailable, ~destructor() = delete;
                         , inline void destroy() noexcept = delete;);

#undef MPARK_VARIANT_DESTRUCTOR

template <typename Traits> class constructor : public destructor<Traits> {
  using super = destructor<Traits>;

public:
  MPARK_INHERITING_CTOR(constructor, super)
  using super::operator=;

protected:
#ifndef MPARK_GENERIC_LAMBDAS
  struct ctor {
    template <typename LhsAlt, typename RhsAlt>
    inline void operator()(LhsAlt &lhs_alt, RhsAlt &&rhs_alt) const {
      constructor::construct_alt(lhs_alt, lib::forward<RhsAlt>(rhs_alt).value);
    }
  };
#endif

  template <std::size_t I, typename T, typename... Args>
  inline static T &construct_alt(alt<I, T> &a, Args &&...args) {
    auto *result = ::new (static_cast<void *>(lib::addressof(a)))
        alt<I, T>(in_place_t{}, lib::forward<Args>(args)...);
    return result->value;
  }

  template <typename Rhs>
  inline static void generic_construct(constructor &lhs, Rhs &&rhs) {
    lhs.destroy();
    if (!rhs.valueless_by_exception()) {
      visitation::alt::visit_alt_at(
          rhs.index(),
#ifdef MPARK_GENERIC_LAMBDAS
          [](auto &lhs_alt, auto &&rhs_alt) {
            constructor::construct_alt(
                lhs_alt, lib::forward<decltype(rhs_alt)>(rhs_alt).value);
          }
#else
          ctor{}
#endif
          ,
          lhs, lib::forward<Rhs>(rhs));
      lhs.index_ = rhs.index_;
    }
  }
};

template <typename Traits, Trait = Traits::move_constructible_trait>
class move_constructor;

#define MPARK_VARIANT_MOVE_CONSTRUCTOR(move_constructible_trait, definition)   \
  template <typename... Ts>                                                    \
  class move_constructor<traits<Ts...>, move_constructible_trait>              \
      : public constructor<traits<Ts...>> {                                    \
    using super = constructor<traits<Ts...>>;                                  \
                                                                               \
  public:                                                                      \
    MPARK_INHERITING_CTOR(move_constructor, super)                             \
    using super::operator=;                                                    \
                                                                               \
    move_constructor(const move_constructor &) = default;                      \
    definition ~move_constructor() = default;                                  \
    move_constructor &operator=(const move_constructor &) = default;           \
    move_constructor &operator=(move_constructor &&) = default;                \
  }

MPARK_VARIANT_MOVE_CONSTRUCTOR(
    Trait::TriviallyAvailable,
    move_constructor(move_constructor &&that) = default;);

MPARK_VARIANT_MOVE_CONSTRUCTOR(
    Trait::Available,
    move_constructor(move_constructor &&that) noexcept(
        lib::all<std::is_nothrow_move_constructible<Ts>::value...>::value)
    : move_constructor(valueless_t{}) {
      this->generic_construct(*this, lib::move(that));
    });

MPARK_VARIANT_MOVE_CONSTRUCTOR(Trait::Unavailable,
                               move_constructor(move_constructor &&) = delete;);

#undef MPARK_VARIANT_MOVE_CONSTRUCTOR

template <typename Traits, Trait = Traits::copy_constructible_trait>
class copy_constructor;

#define MPARK_VARIANT_COPY_CONSTRUCTOR(copy_constructible_trait, definition)   \
  template <typename... Ts>                                                    \
  class copy_constructor<traits<Ts...>, copy_constructible_trait>              \
      : public move_constructor<traits<Ts...>> {                               \
    using super = move_constructor<traits<Ts...>>;                             \
                                                                               \
  public:                                                                      \
    MPARK_INHERITING_CTOR(copy_constructor, super)                             \
    using super::operator=;                                                    \
                                                                               \
    definition copy_constructor(copy_constructor &&) = default;                \
    ~copy_constructor() = default;                                             \
    copy_constructor &operator=(const copy_constructor &) = default;           \
    copy_constructor &operator=(copy_constructor &&) = default;                \
  }

MPARK_VARIANT_COPY_CONSTRUCTOR(
    Trait::TriviallyAvailable,
    copy_constructor(const copy_constructor &that) = default;);

MPARK_VARIANT_COPY_CONSTRUCTOR(
    Trait::Available, copy_constructor(const copy_constructor &that)
    : copy_constructor(valueless_t{}) {
      this->generic_construct(*this, that);
    });

MPARK_VARIANT_COPY_CONSTRUCTOR(
    Trait::Unavailable, copy_constructor(const copy_constructor &) = delete;);

#undef MPARK_VARIANT_COPY_CONSTRUCTOR

template <typename Traits> class assignment : public copy_constructor<Traits> {
  using super = copy_constructor<Traits>;

public:
  MPARK_INHERITING_CTOR(assignment, super)
  using super::operator=;

  template <std::size_t I, typename... Args>
  inline /* auto & */ auto emplace(Args &&...args)
      -> decltype(this->construct_alt(access::base::get_alt<I>(*this),
                                      lib::forward<Args>(args)...)) {
    this->destroy();
    auto &result = this->construct_alt(access::base::get_alt<I>(*this),
                                       lib::forward<Args>(args)...);
    this->index_ = I;
    return result;
  }

protected:
#ifndef MPARK_GENERIC_LAMBDAS
  template <typename That> struct assigner {
    template <typename ThisAlt, typename ThatAlt>
    inline void operator()(ThisAlt &this_alt, ThatAlt &&that_alt) const {
      self->assign_alt(this_alt, lib::forward<ThatAlt>(that_alt).value);
    }
    assignment *self;
  };
#endif

  template <std::size_t I, typename T, typename Arg>
  inline void assign_alt(alt<I, T> &a, Arg &&arg) {
    if (this->index() == I) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
      a.value = lib::forward<Arg>(arg);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    } else {
      struct {
        void operator()(std::true_type) const {
          this_->emplace<I>(lib::forward<Arg>(arg_));
        }
        void operator()(std::false_type) const {
          this_->emplace<I>(T(lib::forward<Arg>(arg_)));
        }
        assignment *this_;
        Arg &&arg_;
      } impl{this, lib::forward<Arg>(arg)};
      impl(lib::bool_constant < std::is_nothrow_constructible<T, Arg>::value ||
           !std::is_nothrow_move_constructible<T>::value > {});
    }
  }

  template <typename That> inline void generic_assign(That &&that) {
    if (this->valueless_by_exception() && that.valueless_by_exception()) {
      // do nothing.
    } else if (that.valueless_by_exception()) {
      this->destroy();
    } else {
      visitation::alt::visit_alt_at(
          that.index(),
#ifdef MPARK_GENERIC_LAMBDAS
          [this](auto &this_alt, auto &&that_alt) {
            this->assign_alt(this_alt,
                             lib::forward<decltype(that_alt)>(that_alt).value);
          }
#else
          assigner<That>{this}
#endif
          ,
          *this, lib::forward<That>(that));
    }
  }
};

template <typename Traits, Trait = Traits::move_assignable_trait>
class move_assignment;

#define MPARK_VARIANT_MOVE_ASSIGNMENT(move_assignable_trait, definition)       \
  template <typename... Ts>                                                    \
  class move_assignment<traits<Ts...>, move_assignable_trait>                  \
      : public assignment<traits<Ts...>> {                                     \
    using super = assignment<traits<Ts...>>;                                   \
                                                                               \
  public:                                                                      \
    MPARK_INHERITING_CTOR(move_assignment, super)                              \
    using super::operator=;                                                    \
                                                                               \
    move_assignment(const move_assignment &) = default;                        \
    move_assignment(move_assignment &&) = default;                             \
    ~move_assignment() = default;                                              \
    move_assignment &operator=(const move_assignment &) = default;             \
    definition                                                                 \
  }

MPARK_VARIANT_MOVE_ASSIGNMENT(
    Trait::TriviallyAvailable,
    move_assignment &operator=(move_assignment &&that) = default;);

MPARK_VARIANT_MOVE_ASSIGNMENT(
    Trait::Available,
    move_assignment &operator=(move_assignment &&that) noexcept(
        lib::all<(std::is_nothrow_move_constructible<Ts>::value &&
                  std::is_nothrow_move_assignable<Ts>::value)...>::value) {
      this->generic_assign(lib::move(that));
      return *this;
    });

MPARK_VARIANT_MOVE_ASSIGNMENT(
    Trait::Unavailable,
    move_assignment &operator=(move_assignment &&) = delete;);

#undef MPARK_VARIANT_MOVE_ASSIGNMENT

template <typename Traits, Trait = Traits::copy_assignable_trait>
class copy_assignment;

#define MPARK_VARIANT_COPY_ASSIGNMENT(copy_assignable_trait, definition)       \
  template <typename... Ts>                                                    \
  class copy_assignment<traits<Ts...>, copy_assignable_trait>                  \
      : public move_assignment<traits<Ts...>> {                                \
    using super = move_assignment<traits<Ts...>>;                              \
                                                                               \
  public:                                                                      \
    MPARK_INHERITING_CTOR(copy_assignment, super)                              \
    using super::operator=;                                                    \
                                                                               \
    copy_assignment(const copy_assignment &) = default;                        \
    copy_assignment(copy_assignment &&) = default;                             \
    ~copy_assignment() = default;                                              \
    definition copy_assignment &operator=(copy_assignment &&) = default;       \
  }

MPARK_VARIANT_COPY_ASSIGNMENT(
    Trait::TriviallyAvailable,
    copy_assignment &operator=(const copy_assignment &that) = default;);

MPARK_VARIANT_COPY_ASSIGNMENT(
    Trait::Available, copy_assignment &operator=(const copy_assignment &that) {
      this->generic_assign(that);
      return *this;
    });

MPARK_VARIANT_COPY_ASSIGNMENT(
    Trait::Unavailable,
    copy_assignment &operator=(const copy_assignment &) = delete;);

#undef MPARK_VARIANT_COPY_ASSIGNMENT

template <typename... Ts> class impl : public copy_assignment<traits<Ts...>> {
  using super = copy_assignment<traits<Ts...>>;

public:
  MPARK_INHERITING_CTOR(impl, super)
  using super::operator=;

  impl(const impl &) = default;
  impl(impl &&) = default;
  ~impl() = default;
  impl &operator=(const impl &) = default;
  impl &operator=(impl &&) = default;

  template <std::size_t I, typename Arg> inline void assign(Arg &&arg) {
    this->assign_alt(access::base::get_alt<I>(*this), lib::forward<Arg>(arg));
  }

  inline void swap(impl &that) {
    if (this->valueless_by_exception() && that.valueless_by_exception()) {
      // do nothing.
    } else if (this->index() == that.index()) {
      visitation::alt::visit_alt_at(
          this->index(),
#ifdef MPARK_GENERIC_LAMBDAS
          [](auto &this_alt, auto &that_alt) {
            using std::swap;
            swap(this_alt.value, that_alt.value);
          }
#else
          swapper{}
#endif
          ,
          *this, that);
    } else {
      impl *lhs = this;
      impl *rhs = lib::addressof(that);
      if (lhs->move_nothrow() && !rhs->move_nothrow()) {
        std::swap(lhs, rhs);
      }
      impl tmp(lib::move(*rhs));
#ifdef MPARK_EXCEPTIONS
      // EXTENSION: When the move construction of `lhs` into `rhs` throws
      // and `tmp` is nothrow move constructible then we move `tmp` back
      // into `rhs` and provide the strong exception safety guarantee.
      try {
        this->generic_construct(*rhs, lib::move(*lhs));
      } catch (...) {
        if (tmp.move_nothrow()) {
          this->generic_construct(*rhs, lib::move(tmp));
        }
        throw;
      }
#else
      this->generic_construct(*rhs, lib::move(*lhs));
#endif
      this->generic_construct(*lhs, lib::move(tmp));
    }
  }

private:
#ifndef MPARK_GENERIC_LAMBDAS
  struct swapper {
    template <typename ThisAlt, typename ThatAlt>
    inline void operator()(ThisAlt &this_alt, ThatAlt &that_alt) const {
      using std::swap;
      swap(this_alt.value, that_alt.value);
    }
  };
#endif

  inline constexpr bool move_nothrow() const {
    return this->valueless_by_exception() ||
           lib::array<bool, sizeof...(Ts)>{{std::is_nothrow_move_constructible<
               Ts>::value...}}[this->index()];
  }
};

#undef MPARK_INHERITING_CTOR

template <typename From, typename To> struct is_non_narrowing_convertible {
  template <typename T> static std::true_type test(T (&&)[1]);

  template <typename T>
  static auto impl(int) -> decltype(test<T>({std::declval<From>()}));

  template <typename> static auto impl(...) -> std::false_type;

  static constexpr bool value = decltype(impl<To>(0))::value;
};

template <typename Arg, std::size_t I, typename T,
          bool = std::is_arithmetic<T>::value, typename = void>
struct overload_leaf {};

template <typename Arg, std::size_t I, typename T>
struct overload_leaf<Arg, I, T, false> {
  using impl = lib::size_constant<I> (*)(T);
  operator impl() const { return nullptr; };
};

template <typename Arg, std::size_t I, typename T>
struct overload_leaf<
    Arg, I, T, true
#if defined(__clang__) || !defined(__GNUC__) || __GNUC__ >= 5
    ,
    lib::enable_if_t<std::is_same<lib::remove_cvref_t<T>, bool>::value
                         ? std::is_same<lib::remove_cvref_t<Arg>, bool>::value
                         : is_non_narrowing_convertible<Arg, T>::value>
#endif
    > {
  using impl = lib::size_constant<I> (*)(T);
  operator impl() const { return nullptr; };
};

template <typename Arg, typename... Ts> struct overload_impl {
private:
  template <typename> struct impl;

  template <std::size_t... Is>
  struct impl<lib::index_sequence<Is...>> : overload_leaf<Arg, Is, Ts>... {};

public:
  using type = impl<lib::index_sequence_for<Ts...>>;
};

template <typename Arg, typename... Ts>
using overload = typename overload_impl<Arg, Ts...>::type;

template <typename Arg, typename... Ts>
using best_match = lib::invoke_result_t<overload<Arg, Ts...>, Arg>;

template <typename T> struct is_in_place_index : std::false_type {};

template <std::size_t I>
struct is_in_place_index<in_place_index_t<I>> : std::true_type {};

template <typename T> struct is_in_place_type : std::false_type {};

template <typename T>
struct is_in_place_type<in_place_type_t<T>> : std::true_type {};

} // namespace detail

template <typename... Ts> class variant {
  static_assert(0 < sizeof...(Ts),
                "variant must consist of at least one alternative.");

  static_assert(lib::all<!std::is_array<Ts>::value...>::value,
                "variant can not have an array type as an alternative.");

  static_assert(lib::all<!std::is_reference<Ts>::value...>::value,
                "variant can not have a reference type as an alternative.");

  static_assert(lib::all<!std::is_void<Ts>::value...>::value,
                "variant can not have a void type as an alternative.");

public:
  template <
      typename Front = lib::type_pack_element_t<0, Ts...>,
      lib::enable_if_t<std::is_default_constructible<Front>::value, int> = 0>
  inline constexpr variant() noexcept(
      std::is_nothrow_default_constructible<Front>::value)
      : impl_(in_place_index_t<0>{}) {}

  variant(const variant &) = default;
  variant(variant &&) = default;

  template <
      typename Arg, typename Decayed = lib::decay_t<Arg>,
      lib::enable_if_t<!std::is_same<Decayed, variant>::value, int> = 0,
      lib::enable_if_t<!detail::is_in_place_index<Decayed>::value, int> = 0,
      lib::enable_if_t<!detail::is_in_place_type<Decayed>::value, int> = 0,
      std::size_t I = detail::best_match<Arg, Ts...>::value,
      typename T = lib::type_pack_element_t<I, Ts...>,
      lib::enable_if_t<std::is_constructible<T, Arg>::value, int> = 0>
  inline constexpr variant(Arg &&arg) noexcept(
      std::is_nothrow_constructible<T, Arg>::value)
      : impl_(in_place_index_t<I>{}, lib::forward<Arg>(arg)) {}

  template <std::size_t I, typename... Args,
            typename T = lib::type_pack_element_t<I, Ts...>,
            lib::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
  inline explicit constexpr variant(
      in_place_index_t<I>,
      Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
      : impl_(in_place_index_t<I>{}, lib::forward<Args>(args)...) {}

  template <
      std::size_t I, typename Up, typename... Args,
      typename T = lib::type_pack_element_t<I, Ts...>,
      lib::enable_if_t<
          std::is_constructible<T, std::initializer_list<Up> &, Args...>::value,
          int> = 0>
  inline explicit constexpr variant(
      in_place_index_t<I>, std::initializer_list<Up> il,
      Args &&...args) noexcept(std::
                                   is_nothrow_constructible<
                                       T, std::initializer_list<Up> &,
                                       Args...>::value)
      : impl_(in_place_index_t<I>{}, il, lib::forward<Args>(args)...) {}

  template <typename T, typename... Args,
            std::size_t I = detail::find_index_sfinae<T, Ts...>::value,
            lib::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
  inline explicit constexpr variant(
      in_place_type_t<T>,
      Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
      : impl_(in_place_index_t<I>{}, lib::forward<Args>(args)...) {}

  template <
      typename T, typename Up, typename... Args,
      std::size_t I = detail::find_index_sfinae<T, Ts...>::value,
      lib::enable_if_t<
          std::is_constructible<T, std::initializer_list<Up> &, Args...>::value,
          int> = 0>
  inline explicit constexpr variant(
      in_place_type_t<T>, std::initializer_list<Up> il,
      Args &&...args) noexcept(std::
                                   is_nothrow_constructible<
                                       T, std::initializer_list<Up> &,
                                       Args...>::value)
      : impl_(in_place_index_t<I>{}, il, lib::forward<Args>(args)...) {}

  ~variant() = default;

  variant &operator=(const variant &) = default;
  variant &operator=(variant &&) = default;

  template <typename Arg,
            lib::enable_if_t<!std::is_same<lib::decay_t<Arg>, variant>::value,
                             int> = 0,
            std::size_t I = detail::best_match<Arg, Ts...>::value,
            typename T = lib::type_pack_element_t<I, Ts...>,
            lib::enable_if_t<(std::is_assignable<T &, Arg>::value &&
                              std::is_constructible<T, Arg>::value),
                             int> = 0>
  inline variant &operator=(Arg &&arg) noexcept(
      (std::is_nothrow_assignable<T &, Arg>::value &&
       std::is_nothrow_constructible<T, Arg>::value)) {
    impl_.template assign<I>(lib::forward<Arg>(arg));
    return *this;
  }

  template <std::size_t I, typename... Args,
            typename T = lib::type_pack_element_t<I, Ts...>,
            lib::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
  inline T &emplace(Args &&...args) {
    return impl_.template emplace<I>(lib::forward<Args>(args)...);
  }

  template <
      std::size_t I, typename Up, typename... Args,
      typename T = lib::type_pack_element_t<I, Ts...>,
      lib::enable_if_t<
          std::is_constructible<T, std::initializer_list<Up> &, Args...>::value,
          int> = 0>
  inline T &emplace(std::initializer_list<Up> il, Args &&...args) {
    return impl_.template emplace<I>(il, lib::forward<Args>(args)...);
  }

  template <typename T, typename... Args,
            std::size_t I = detail::find_index_sfinae<T, Ts...>::value,
            lib::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
  inline T &emplace(Args &&...args) {
    return impl_.template emplace<I>(lib::forward<Args>(args)...);
  }

  template <
      typename T, typename Up, typename... Args,
      std::size_t I = detail::find_index_sfinae<T, Ts...>::value,
      lib::enable_if_t<
          std::is_constructible<T, std::initializer_list<Up> &, Args...>::value,
          int> = 0>
  inline T &emplace(std::initializer_list<Up> il, Args &&...args) {
    return impl_.template emplace<I>(il, lib::forward<Args>(args)...);
  }

  inline constexpr bool valueless_by_exception() const noexcept {
    return impl_.valueless_by_exception();
  }

  inline constexpr std::size_t index() const noexcept { return impl_.index(); }

  template <
      bool Dummy = true,
      lib::enable_if_t<
          lib::all<Dummy, (lib::dependent_type<std::is_move_constructible<Ts>,
                                               Dummy>::value &&
                           lib::dependent_type<lib::is_swappable<Ts>,
                                               Dummy>::value)...>::value,
          int> = 0>
  inline void swap(variant &that) noexcept(
      lib::all<(std::is_nothrow_move_constructible<Ts>::value &&
                lib::is_nothrow_swappable<Ts>::value)...>::value) {
    impl_.swap(that.impl_);
  }

private:
  detail::impl<Ts...> impl_;

  friend struct detail::access::variant;
  friend struct detail::visitation::variant;
};

template <std::size_t I, typename... Ts>
inline constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  return v.index() == I;
}

template <typename T, typename... Ts>
inline constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  return holds_alternative<detail::find_index_checked<T, Ts...>::value>(v);
}

namespace detail {
template <std::size_t I, typename V> struct generic_get_impl {
  constexpr generic_get_impl(int) noexcept {}

  constexpr AUTO_REFREF operator()(V &&v) const
      AUTO_REFREF_RETURN(access::variant::get_alt<I>(lib::forward<V>(v)).value)
};

template <std::size_t I, typename V>
inline constexpr AUTO_REFREF generic_get(V &&v)
    AUTO_REFREF_RETURN(generic_get_impl<I, V>(holds_alternative<I>(v)
                                                  ? 0
                                                  : (throw_bad_variant_access(),
                                                     0))(lib::forward<V>(v)))
} // namespace detail

template <std::size_t I, typename... Ts>
inline constexpr variant_alternative_t<I, variant<Ts...>> &
get(variant<Ts...> &v) {
  return detail::generic_get<I>(v);
}

template <std::size_t I, typename... Ts>
inline constexpr variant_alternative_t<I, variant<Ts...>> &&
get(variant<Ts...> &&v) {
  return detail::generic_get<I>(lib::move(v));
}

template <std::size_t I, typename... Ts>
inline constexpr const variant_alternative_t<I, variant<Ts...>> &
get(const variant<Ts...> &v) {
  return detail::generic_get<I>(v);
}

template <std::size_t I, typename... Ts>
inline constexpr const variant_alternative_t<I, variant<Ts...>> &&
get(const variant<Ts...> &&v) {
  return detail::generic_get<I>(lib::move(v));
}

template <typename T, typename... Ts>
inline constexpr T &get(variant<Ts...> &v) {
  return get<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr T &&get(variant<Ts...> &&v) {
  return get<detail::find_index_checked<T, Ts...>::value>(lib::move(v));
}

template <typename T, typename... Ts>
inline constexpr const T &get(const variant<Ts...> &v) {
  return get<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr const T &&get(const variant<Ts...> &&v) {
  return get<detail::find_index_checked<T, Ts...>::value>(lib::move(v));
}

namespace detail {

template <std::size_t I, typename V>
inline constexpr /* auto * */ AUTO generic_get_if(V *v) noexcept
    AUTO_RETURN(v &&holds_alternative<I>(*v)
                    ? lib::addressof(access::variant::get_alt<I>(*v).value)
                    : nullptr)

} // namespace detail

template <std::size_t I, typename... Ts>
inline constexpr lib::add_pointer_t<variant_alternative_t<I, variant<Ts...>>>
get_if(variant<Ts...> *v) noexcept {
  return detail::generic_get_if<I>(v);
}

template <std::size_t I, typename... Ts>
inline constexpr lib::add_pointer_t<
    const variant_alternative_t<I, variant<Ts...>>>
get_if(const variant<Ts...> *v) noexcept {
  return detail::generic_get_if<I>(v);
}

template <typename T, typename... Ts>
inline constexpr lib::add_pointer_t<T> get_if(variant<Ts...> *v) noexcept {
  return get_if<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr lib::add_pointer_t<const T>
get_if(const variant<Ts...> *v) noexcept {
  return get_if<detail::find_index_checked<T, Ts...>::value>(v);
}

namespace detail {
template <typename RelOp> struct convert_to_bool {
  template <typename Lhs, typename Rhs>
  inline constexpr bool operator()(Lhs &&lhs, Rhs &&rhs) const {
    static_assert(
        std::is_convertible<lib::invoke_result_t<RelOp, Lhs, Rhs>, bool>::value,
        "relational operators must return a type"
        " implicitly convertible to bool");
    return lib::invoke(RelOp{}, lib::forward<Lhs>(lhs), lib::forward<Rhs>(rhs));
  }
};
} // namespace detail

template <typename... Ts>
inline constexpr bool operator==(const variant<Ts...> &lhs,
                                 const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using equal_to = detail::convert_to_bool<lib::equal_to>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (lhs.index() != rhs.index())
    return false;
  if (lhs.valueless_by_exception())
    return true;
  return variant::visit_value_at(lhs.index(), equal_to{}, lhs, rhs);
#else
  return lhs.index() == rhs.index() &&
         (lhs.valueless_by_exception() ||
          variant::visit_value_at(lhs.index(), equal_to{}, lhs, rhs));
#endif
}

template <typename... Ts>
inline constexpr bool operator!=(const variant<Ts...> &lhs,
                                 const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using not_equal_to = detail::convert_to_bool<lib::not_equal_to>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (lhs.index() != rhs.index())
    return true;
  if (lhs.valueless_by_exception())
    return false;
  return variant::visit_value_at(lhs.index(), not_equal_to{}, lhs, rhs);
#else
  return lhs.index() != rhs.index() ||
         (!lhs.valueless_by_exception() &&
          variant::visit_value_at(lhs.index(), not_equal_to{}, lhs, rhs));
#endif
}

template <typename... Ts>
inline constexpr bool operator<(const variant<Ts...> &lhs,
                                const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using less = detail::convert_to_bool<lib::less>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (rhs.valueless_by_exception())
    return false;
  if (lhs.valueless_by_exception())
    return true;
  if (lhs.index() < rhs.index())
    return true;
  if (lhs.index() > rhs.index())
    return false;
  return variant::visit_value_at(lhs.index(), less{}, lhs, rhs);
#else
  return !rhs.valueless_by_exception() &&
         (lhs.valueless_by_exception() || lhs.index() < rhs.index() ||
          (lhs.index() == rhs.index() &&
           variant::visit_value_at(lhs.index(), less{}, lhs, rhs)));
#endif
}

template <typename... Ts>
inline constexpr bool operator>(const variant<Ts...> &lhs,
                                const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using greater = detail::convert_to_bool<lib::greater>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (lhs.valueless_by_exception())
    return false;
  if (rhs.valueless_by_exception())
    return true;
  if (lhs.index() > rhs.index())
    return true;
  if (lhs.index() < rhs.index())
    return false;
  return variant::visit_value_at(lhs.index(), greater{}, lhs, rhs);
#else
  return !lhs.valueless_by_exception() &&
         (rhs.valueless_by_exception() || lhs.index() > rhs.index() ||
          (lhs.index() == rhs.index() &&
           variant::visit_value_at(lhs.index(), greater{}, lhs, rhs)));
#endif
}

template <typename... Ts>
inline constexpr bool operator<=(const variant<Ts...> &lhs,
                                 const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using less_equal = detail::convert_to_bool<lib::less_equal>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (lhs.valueless_by_exception())
    return true;
  if (rhs.valueless_by_exception())
    return false;
  if (lhs.index() < rhs.index())
    return true;
  if (lhs.index() > rhs.index())
    return false;
  return variant::visit_value_at(lhs.index(), less_equal{}, lhs, rhs);
#else
  return lhs.valueless_by_exception() ||
         (!rhs.valueless_by_exception() &&
          (lhs.index() < rhs.index() ||
           (lhs.index() == rhs.index() &&
            variant::visit_value_at(lhs.index(), less_equal{}, lhs, rhs))));
#endif
}

template <typename... Ts>
inline constexpr bool operator>=(const variant<Ts...> &lhs,
                                 const variant<Ts...> &rhs) {
  using detail::visitation::variant;
  using greater_equal = detail::convert_to_bool<lib::greater_equal>;
#ifdef MPARK_CPP14_CONSTEXPR
  if (rhs.valueless_by_exception())
    return true;
  if (lhs.valueless_by_exception())
    return false;
  if (lhs.index() > rhs.index())
    return true;
  if (lhs.index() < rhs.index())
    return false;
  return variant::visit_value_at(lhs.index(), greater_equal{}, lhs, rhs);
#else
  return rhs.valueless_by_exception() ||
         (!lhs.valueless_by_exception() &&
          (lhs.index() > rhs.index() ||
           (lhs.index() == rhs.index() &&
            variant::visit_value_at(lhs.index(), greater_equal{}, lhs, rhs))));
#endif
}

struct monostate {};

inline constexpr bool operator<(monostate, monostate) noexcept { return false; }

inline constexpr bool operator>(monostate, monostate) noexcept { return false; }

inline constexpr bool operator<=(monostate, monostate) noexcept { return true; }

inline constexpr bool operator>=(monostate, monostate) noexcept { return true; }

inline constexpr bool operator==(monostate, monostate) noexcept { return true; }

inline constexpr bool operator!=(monostate, monostate) noexcept {
  return false;
}

#ifdef MPARK_CPP14_CONSTEXPR
namespace detail {

inline constexpr bool any(std::initializer_list<bool> bs) {
  for (bool b : bs) {
    if (b) {
      return true;
    }
  }
  return false;
}

} // namespace detail

template <typename Visitor, typename... Vs>
inline constexpr decltype(auto) visit(Visitor &&visitor, Vs &&...vs) {
  return (!detail::any({vs.valueless_by_exception()...})
              ? (void)0
              : throw_bad_variant_access()),
         detail::visitation::variant::visit_value(
             lib::forward<Visitor>(visitor), lib::forward<Vs>(vs)...);
}
#else
namespace detail {

template <std::size_t N>
inline constexpr bool all_impl(const lib::array<bool, N> &bs, std::size_t idx) {
  return idx >= N || (bs[idx] && all_impl(bs, idx + 1));
}

template <std::size_t N>
inline constexpr bool all(const lib::array<bool, N> &bs) {
  return all_impl(bs, 0);
}

} // namespace detail

template <typename Visitor, typename... Vs>
inline constexpr DECLTYPE_AUTO visit(Visitor &&visitor, Vs &&...vs)
    DECLTYPE_AUTO_RETURN(
        (detail::all(lib::array<bool, sizeof...(Vs)>{
             {!vs.valueless_by_exception()...}})
             ? (void)0
             : throw_bad_variant_access()),
        detail::visitation::variant::visit_value(lib::forward<Visitor>(visitor),
                                                 lib::forward<Vs>(vs)...))
#endif

template <typename... Ts>
inline auto swap(variant<Ts...> &lhs,
                 variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs)))
    -> decltype(lhs.swap(rhs)) {
  lhs.swap(rhs);
}

namespace detail {

template <typename T, typename...> using enabled_type = T;

namespace hash {

template <typename H, typename K> constexpr bool meets_requirements() noexcept {
  return std::is_copy_constructible<H>::value &&
         std::is_move_constructible<H>::value &&
         lib::is_invocable_r<std::size_t, H, const K &>::value;
}

template <typename K> constexpr bool is_enabled() noexcept {
  using H = std::hash<K>;
  return meets_requirements<H, K>() &&
         std::is_default_constructible<H>::value &&
         std::is_copy_assignable<H>::value && std::is_move_assignable<H>::value;
}

} // namespace hash

} // namespace detail

#undef AUTO
#undef AUTO_RETURN

#undef AUTO_REFREF
#undef AUTO_REFREF_RETURN

#undef DECLTYPE_AUTO
#undef DECLTYPE_AUTO_RETURN

} // namespace mpark

namespace std {

template <typename... Ts>
struct hash<mpark::detail::enabled_type<
    mpark::variant<Ts...>,
    mpark::lib::enable_if_t<mpark::lib::all<mpark::detail::hash::is_enabled<
        mpark::lib::remove_const_t<Ts>>()...>::value>>> {
  using argument_type = mpark::variant<Ts...>;
  using result_type = std::size_t;

  inline result_type operator()(const argument_type &v) const {
    using mpark::detail::visitation::variant;
    std::size_t result =
        v.valueless_by_exception()
            ? 299792458 // Random value chosen by the universe upon creation
            : variant::visit_alt(
#ifdef MPARK_GENERIC_LAMBDAS
                  [](const auto &alt) {
                    using alt_type = mpark::lib::decay_t<decltype(alt)>;
                    using value_type = mpark::lib::remove_const_t<
                        typename alt_type::value_type>;
                    return hash<value_type>{}(alt.value);
                  }
#else
                  hasher{}
#endif
                  ,
                  v);
    return hash_combine(result, hash<std::size_t>{}(v.index()));
  }

private:
#ifndef MPARK_GENERIC_LAMBDAS
  struct hasher {
    template <typename Alt>
    inline std::size_t operator()(const Alt &alt) const {
      using alt_type = mpark::lib::decay_t<Alt>;
      using value_type =
          mpark::lib::remove_const_t<typename alt_type::value_type>;
      return hash<value_type>{}(alt.value);
    }
  };
#endif

  static std::size_t hash_combine(std::size_t lhs, std::size_t rhs) {
    return lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
  }
};

template <> struct hash<mpark::monostate> {
  using argument_type = mpark::monostate;
  using result_type = std::size_t;

  inline result_type operator()(const argument_type &) const noexcept {
    return 66740831; // return a fundamentally attractive random value.
  }
};

} // namespace std

// Copyright 2017-2020 by Martin Moene
//
// string-view lite, a C++17-like string_view for C++98 and later.
// For more information see https://github.com/martinmoene/string-view-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_SV_LITE_H_INCLUDED
#define NONSTD_SV_LITE_H_INCLUDED

#define string_view_lite_MAJOR 1
#define string_view_lite_MINOR 7
#define string_view_lite_PATCH 0

#define string_view_lite_VERSION                                               \
  nssv_STRINGIFY(string_view_lite_MAJOR) "." nssv_STRINGIFY(                   \
      string_view_lite_MINOR) "." nssv_STRINGIFY(string_view_lite_PATCH)

#define nssv_STRINGIFY(x) nssv_STRINGIFY_(x)
#define nssv_STRINGIFY_(x) #x

// string-view lite configuration:

#define nssv_STRING_VIEW_DEFAULT 0
#define nssv_STRING_VIEW_NONSTD 1
#define nssv_STRING_VIEW_STD 2

// tweak header support:

#ifdef __has_include
#if __has_include(<nonstd/string_view.tweak.hpp>)
#include <nonstd/string_view.tweak.hpp>
#endif
#define nssv_HAVE_TWEAK_HEADER 1
#else
#define nssv_HAVE_TWEAK_HEADER 0
// # pragma message("string_view.hpp: Note: Tweak header not supported.")
#endif

// string_view selection and configuration:

#if !defined(nssv_CONFIG_SELECT_STRING_VIEW)
#define nssv_CONFIG_SELECT_STRING_VIEW                                         \
  (nssv_HAVE_STD_STRING_VIEW ? nssv_STRING_VIEW_STD : nssv_STRING_VIEW_NONSTD)
#endif

#ifndef nssv_CONFIG_STD_SV_OPERATOR
#define nssv_CONFIG_STD_SV_OPERATOR 0
#endif

#ifndef nssv_CONFIG_USR_SV_OPERATOR
#define nssv_CONFIG_USR_SV_OPERATOR 1
#endif

#ifdef nssv_CONFIG_CONVERSION_STD_STRING
#define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS                        \
  nssv_CONFIG_CONVERSION_STD_STRING
#define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS                       \
  nssv_CONFIG_CONVERSION_STD_STRING
#endif

#ifndef nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
#define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS 1
#endif

#ifndef nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
#define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS 1
#endif

#ifndef nssv_CONFIG_NO_STREAM_INSERTION
#define nssv_CONFIG_NO_STREAM_INSERTION 0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef nssv_CONFIG_NO_EXCEPTIONS
#if defined(_MSC_VER)
#include <cstddef> // for _HAS_EXCEPTIONS
#endif
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || (_HAS_EXCEPTIONS)
#define nssv_CONFIG_NO_EXCEPTIONS 0
#else
#define nssv_CONFIG_NO_EXCEPTIONS 1
#endif
#endif

// C++ language version detection (C++23 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef nssv_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define nssv_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define nssv_CPLUSPLUS __cplusplus
#endif
#endif

#define nssv_CPP98_OR_GREATER (nssv_CPLUSPLUS >= 199711L)
#define nssv_CPP11_OR_GREATER (nssv_CPLUSPLUS >= 201103L)
#define nssv_CPP11_OR_GREATER_ (nssv_CPLUSPLUS >= 201103L)
#define nssv_CPP14_OR_GREATER (nssv_CPLUSPLUS >= 201402L)
#define nssv_CPP17_OR_GREATER (nssv_CPLUSPLUS >= 201703L)
#define nssv_CPP20_OR_GREATER (nssv_CPLUSPLUS >= 202002L)
#define nssv_CPP23_OR_GREATER (nssv_CPLUSPLUS >= 202300L)

// use C++17 std::string_view if available and requested:

#if nssv_CPP17_OR_GREATER && defined(__has_include)
#if __has_include(<string_view> )
#define nssv_HAVE_STD_STRING_VIEW 1
#else
#define nssv_HAVE_STD_STRING_VIEW 0
#endif
#else
#define nssv_HAVE_STD_STRING_VIEW 0
#endif

#define nssv_USES_STD_STRING_VIEW                                              \
  ((nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_STD) ||                 \
   ((nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_DEFAULT) &&            \
    nssv_HAVE_STD_STRING_VIEW))

#define nssv_HAVE_STARTS_WITH                                                  \
  (nssv_CPP20_OR_GREATER || !nssv_USES_STD_STRING_VIEW)
#define nssv_HAVE_ENDS_WITH nssv_HAVE_STARTS_WITH

//
// Use C++17 std::string_view:
//

#if nssv_USES_STD_STRING_VIEW

#include <string_view>

// Extensions for std::string:

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

template <class CharT, class Traits, class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator>
to_string(std::basic_string_view<CharT, Traits> v,
          Allocator const &a = Allocator()) {
  return std::basic_string<CharT, Traits, Allocator>(v.begin(), v.end(), a);
}

template <class CharT, class Traits, class Allocator>
std::basic_string_view<CharT, Traits>
to_string_view(std::basic_string<CharT, Traits, Allocator> const &s) {
  return std::basic_string_view<CharT, Traits>(s.data(), s.size());
}

// Literal operators sv and _sv:

#if nssv_CONFIG_STD_SV_OPERATOR

using namespace std::literals::string_view_literals;

#endif

#if nssv_CONFIG_USR_SV_OPERATOR

inline namespace literals {
inline namespace string_view_literals {

constexpr std::string_view operator"" _sv(const char *str,
                                          size_t len) noexcept // (1)
{
  return std::string_view{str, len};
}

constexpr std::u16string_view operator"" _sv(const char16_t *str,
                                             size_t len) noexcept // (2)
{
  return std::u16string_view{str, len};
}

constexpr std::u32string_view operator"" _sv(const char32_t *str,
                                             size_t len) noexcept // (3)
{
  return std::u32string_view{str, len};
}

constexpr std::wstring_view operator"" _sv(const wchar_t *str,
                                           size_t len) noexcept // (4)
{
  return std::wstring_view{str, len};
}

} // namespace string_view_literals
} // namespace literals

#endif // nssv_CONFIG_USR_SV_OPERATOR

} // namespace nonstd

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

using std::basic_string_view;
using std::string_view;
using std::u16string_view;
using std::u32string_view;
using std::wstring_view;

// literal "sv" and "_sv", see above

using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;

using std::operator<<;

} // namespace nonstd

#else // nssv_HAVE_STD_STRING_VIEW

//
// Before C++17: use string_view lite:
//

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  nssv_COMPILER_MSVC_VERSION ==  60  (Visual
// Studio 6.0) MSVC++  7.0  _MSC_VER == 1300  nssv_COMPILER_MSVC_VERSION ==  70
// (Visual Studio .NET 2002) MSVC++  7.1  _MSC_VER == 1310
// nssv_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003) MSVC++  8.0
// _MSC_VER == 1400  nssv_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  nssv_COMPILER_MSVC_VERSION ==  90  (Visual
// Studio 2008) MSVC++ 10.0  _MSC_VER == 1600  nssv_COMPILER_MSVC_VERSION == 100
// (Visual Studio 2010) MSVC++ 11.0  _MSC_VER == 1700 nssv_COMPILER_MSVC_VERSION
// == 110  (Visual Studio 2012) MSVC++ 12.0  _MSC_VER == 1800
// nssv_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013) MSVC++ 14.0  _MSC_VER
// == 1900  nssv_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015) MSVC++ 14.1
// _MSC_VER >= 1910  nssv_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  nssv_COMPILER_MSVC_VERSION == 142  (Visual
// Studio 2019)

#if defined(_MSC_VER) && !defined(__clang__)
#define nssv_COMPILER_MSVC_VER (_MSC_VER)
#define nssv_COMPILER_MSVC_VERSION                                             \
  (_MSC_VER / 10 - 10 * (5 + (_MSC_VER < 1900)))
#else
#define nssv_COMPILER_MSVC_VER 0
#define nssv_COMPILER_MSVC_VERSION 0
#endif

#define nssv_COMPILER_VERSION(major, minor, patch)                             \
  (10 * (10 * (major) + (minor)) + (patch))

#if defined(__apple_build_version__)
#define nssv_COMPILER_APPLECLANG_VERSION                                       \
  nssv_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#define nssv_COMPILER_CLANG_VERSION 0
#elif defined(__clang__)
#define nssv_COMPILER_APPLECLANG_VERSION 0
#define nssv_COMPILER_CLANG_VERSION                                            \
  nssv_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
#define nssv_COMPILER_APPLECLANG_VERSION 0
#define nssv_COMPILER_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define nssv_COMPILER_GNUC_VERSION                                             \
  nssv_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define nssv_COMPILER_GNUC_VERSION 0
#endif

// half-open range [lo..hi):
#define nssv_BETWEEN(v, lo, hi) ((lo) <= (v) && (v) < (hi))

// Presence of language and library features:

#ifdef _HAS_CPP0X
#define nssv_HAS_CPP0X _HAS_CPP0X
#else
#define nssv_HAS_CPP0X 0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for variant-lite:

#if nssv_COMPILER_MSVC_VER >= 1900
#undef nssv_CPP11_OR_GREATER
#define nssv_CPP11_OR_GREATER 1
#endif

#define nssv_CPP11_90 (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1500)
#define nssv_CPP11_100                                                         \
  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1600)
#define nssv_CPP11_110                                                         \
  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1700)
#define nssv_CPP11_120                                                         \
  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1800)
#define nssv_CPP11_140                                                         \
  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1900)
#define nssv_CPP11_141                                                         \
  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1910)

#define nssv_CPP14_000 (nssv_CPP14_OR_GREATER)
#define nssv_CPP17_000 (nssv_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define nssv_HAVE_CONSTEXPR_11 nssv_CPP11_140
#define nssv_HAVE_EXPLICIT_CONVERSION nssv_CPP11_140
#define nssv_HAVE_INLINE_NAMESPACE nssv_CPP11_140
#define nssv_HAVE_IS_DEFAULT nssv_CPP11_140
#define nssv_HAVE_IS_DELETE nssv_CPP11_140
#define nssv_HAVE_NOEXCEPT nssv_CPP11_140
#define nssv_HAVE_NULLPTR nssv_CPP11_100
#define nssv_HAVE_REF_QUALIFIER nssv_CPP11_140
#define nssv_HAVE_UNICODE_LITERALS nssv_CPP11_140
#define nssv_HAVE_USER_DEFINED_LITERALS nssv_CPP11_140
#define nssv_HAVE_WCHAR16_T nssv_CPP11_100
#define nssv_HAVE_WCHAR32_T nssv_CPP11_100

#if !((nssv_CPP11_OR_GREATER && nssv_COMPILER_CLANG_VERSION) ||                \
      nssv_BETWEEN(nssv_COMPILER_CLANG_VERSION, 300, 400))
#define nssv_HAVE_STD_DEFINED_LITERALS nssv_CPP11_140
#else
#define nssv_HAVE_STD_DEFINED_LITERALS 0
#endif

// Presence of C++14 language features:

#define nssv_HAVE_CONSTEXPR_14 nssv_CPP14_000

// Presence of C++17 language features:

#define nssv_HAVE_NODISCARD nssv_CPP17_000

// Presence of C++ library features:

#define nssv_HAVE_STD_HASH nssv_CPP11_120

// Presence of compiler intrinsics:

// Providing char-type specializations for compare() and length() that
// use compiler intrinsics can improve compile- and run-time performance.
//
// The challenge is in using the right combinations of builtin availability
// and its constexpr-ness.
//
// | compiler | __builtin_memcmp (constexpr) | memcmp  (constexpr) |
// |----------|------------------------------|---------------------|
// | clang    | 4.0              (>= 4.0   ) | any     (?        ) |
// | clang-a  | 9.0              (>= 9.0   ) | any     (?        ) |
// | gcc      | any              (constexpr) | any     (?        ) |
// | msvc     | >= 14.2 C++17    (>= 14.2  ) | any     (?        ) |

#define nssv_HAVE_BUILTIN_VER                                                  \
  ((nssv_CPP17_000 && nssv_COMPILER_MSVC_VERSION >= 142) ||                    \
   nssv_COMPILER_GNUC_VERSION > 0 || nssv_COMPILER_CLANG_VERSION >= 400 ||     \
   nssv_COMPILER_APPLECLANG_VERSION >= 900)
#define nssv_HAVE_BUILTIN_CE (nssv_HAVE_BUILTIN_VER)

#define nssv_HAVE_BUILTIN_MEMCMP                                               \
  ((nssv_HAVE_CONSTEXPR_14 && nssv_HAVE_BUILTIN_CE) || !nssv_HAVE_CONSTEXPR_14)
#define nssv_HAVE_BUILTIN_STRLEN                                               \
  ((nssv_HAVE_CONSTEXPR_11 && nssv_HAVE_BUILTIN_CE) || !nssv_HAVE_CONSTEXPR_11)

#ifdef __has_builtin
#define nssv_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define nssv_HAVE_BUILTIN(x) 0
#endif

#if nssv_HAVE_BUILTIN(__builtin_memcmp) || nssv_HAVE_BUILTIN_VER
#define nssv_BUILTIN_MEMCMP __builtin_memcmp
#else
#define nssv_BUILTIN_MEMCMP memcmp
#endif

#if nssv_HAVE_BUILTIN(__builtin_strlen) || nssv_HAVE_BUILTIN_VER
#define nssv_BUILTIN_STRLEN __builtin_strlen
#else
#define nssv_BUILTIN_STRLEN strlen
#endif

// C++ feature usage:

#if nssv_HAVE_CONSTEXPR_11
#define nssv_constexpr constexpr
#else
#define nssv_constexpr /*constexpr*/
#endif

#if nssv_HAVE_CONSTEXPR_14
#define nssv_constexpr14 constexpr
#else
#define nssv_constexpr14 /*constexpr*/
#endif

#if nssv_HAVE_EXPLICIT_CONVERSION
#define nssv_explicit explicit
#else
#define nssv_explicit /*explicit*/
#endif

#if nssv_HAVE_INLINE_NAMESPACE
#define nssv_inline_ns inline
#else
#define nssv_inline_ns /*inline*/
#endif

#if nssv_HAVE_NOEXCEPT
#define nssv_noexcept noexcept
#else
#define nssv_noexcept /*noexcept*/
#endif

// #if nssv_HAVE_REF_QUALIFIER
// # define nssv_ref_qual  &
// # define nssv_refref_qual  &&
// #else
// # define nssv_ref_qual  /*&*/
// # define nssv_refref_qual  /*&&*/
// #endif

#if nssv_HAVE_NULLPTR
#define nssv_nullptr nullptr
#else
#define nssv_nullptr NULL
#endif

#if nssv_HAVE_NODISCARD
#define nssv_nodiscard [[nodiscard]]
#else
#define nssv_nodiscard /*[[nodiscard]]*/
#endif

// Additional includes:

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <string> // std::char_traits<>

#if !nssv_CONFIG_NO_STREAM_INSERTION
#include <ostream>
#endif

#if !nssv_CONFIG_NO_EXCEPTIONS
#include <stdexcept>
#endif

#if nssv_CPP11_OR_GREATER
#include <type_traits>
#endif

// Clang, GNUC, MSVC warning suppression macros:

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-user-defined-literal"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"
#elif nssv_COMPILER_GNUC_VERSION >= 480
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"
#endif // __clang__

#if nssv_COMPILER_MSVC_VERSION >= 140
#define nssv_SUPPRESS_MSGSL_WARNING(expr) [[gsl::suppress(expr)]]
#define nssv_SUPPRESS_MSVC_WARNING(code, descr)                                \
  __pragma(warning(suppress : code))
#define nssv_DISABLE_MSVC_WARNINGS(codes)                                      \
  __pragma(warning(push)) __pragma(warning(disable : codes))
#else
#define nssv_SUPPRESS_MSGSL_WARNING(expr)
#define nssv_SUPPRESS_MSVC_WARNING(code, descr)
#define nssv_DISABLE_MSVC_WARNINGS(codes)
#endif

#if defined(__clang__)
#define nssv_RESTORE_WARNINGS() _Pragma("clang diagnostic pop")
#elif nssv_COMPILER_GNUC_VERSION >= 480
#define nssv_RESTORE_WARNINGS() _Pragma("GCC diagnostic pop")
#elif nssv_COMPILER_MSVC_VERSION >= 140
#define nssv_RESTORE_WARNINGS() __pragma(warning(pop))
#else
#define nssv_RESTORE_WARNINGS()
#endif

// Suppress the following MSVC (GSL) warnings:
// - C4455, non-gsl   : 'operator ""sv': literal suffix identifiers that do not
//                      start with an underscore are reserved
// - C26472, gsl::t.1 : don't use a static_cast for arithmetic conversions;
//                      use brace initialization, gsl::narrow_cast or gsl::narow
// - C26481: gsl::b.1 : don't use pointer arithmetic. Use span instead

nssv_DISABLE_MSVC_WARNINGS(4455 26481 26472)
    // nssv_DISABLE_CLANG_WARNINGS( "-Wuser-defined-literals" )
    // nssv_DISABLE_GNUC_WARNINGS( -Wliteral-suffix )

    namespace nonstd {
  namespace sv_lite {

  //
  // basic_string_view declaration:
  //

  template <class CharT, class Traits = std::char_traits<CharT>>
  class basic_string_view;

  namespace detail {

  // support constexpr comparison in C++14;
  // for C++17 and later, use provided traits:

  template <typename CharT>
  inline nssv_constexpr14 int compare(CharT const *s1, CharT const *s2,
                                      std::size_t count) {
    while (count-- != 0) {
      if (*s1 < *s2)
        return -1;
      if (*s1 > *s2)
        return +1;
      ++s1;
      ++s2;
    }
    return 0;
  }

#if nssv_HAVE_BUILTIN_MEMCMP

  // specialization of compare() for char, see also generic compare() above:

  inline nssv_constexpr14 int compare(char const *s1, char const *s2,
                                      std::size_t count) {
    return nssv_BUILTIN_MEMCMP(s1, s2, count);
  }

#endif

#if nssv_HAVE_BUILTIN_STRLEN

  // specialization of length() for char, see also generic length() further
  // below:

  inline nssv_constexpr std::size_t length(char const *s) {
    return nssv_BUILTIN_STRLEN(s);
  }

#endif

#if defined(__OPTIMIZE__)

  // gcc, clang provide __OPTIMIZE__
  // Expect tail call optimization to make length() non-recursive:

  template <typename CharT>
  inline nssv_constexpr std::size_t length(CharT *s, std::size_t result = 0) {
    return *s == '\0' ? result : length(s + 1, result + 1);
  }

#else // OPTIMIZE

  // non-recursive:

  template <typename CharT>
  inline nssv_constexpr14 std::size_t length(CharT *s) {
    std::size_t result = 0;
    while (*s++ != '\0') {
      ++result;
    }
    return result;
  }

#endif // OPTIMIZE

#if nssv_CPP11_OR_GREATER && !nssv_CPP17_OR_GREATER
#if defined(__OPTIMIZE__)

  // gcc, clang provide __OPTIMIZE__
  // Expect tail call optimization to make search() non-recursive:

  template <class CharT, class Traits = std::char_traits<CharT>>
  constexpr const CharT *search(basic_string_view<CharT, Traits> haystack,
                                basic_string_view<CharT, Traits> needle) {
    return haystack.starts_with(needle) ? haystack.begin()
           : haystack.empty()           ? haystack.end()
                                        : search(haystack.substr(1), needle);
  }

#else // OPTIMIZE

  // non-recursive:

  template <class CharT, class Traits = std::char_traits<CharT>>
  constexpr const CharT *search(basic_string_view<CharT, Traits> haystack,
                                basic_string_view<CharT, Traits> needle) {
    return std::search(haystack.begin(), haystack.end(), needle.begin(),
                       needle.end());
  }

#endif // OPTIMIZE
#endif // nssv_CPP11_OR_GREATER && ! nssv_CPP17_OR_GREATER

  } // namespace detail

  //
  // basic_string_view:
  //

  template <class CharT, class Traits /* = std::char_traits<CharT> */
            >
  class basic_string_view {
  public:
    // Member types:

    typedef Traits traits_type;
    typedef CharT value_type;

    typedef CharT *pointer;
    typedef CharT const *const_pointer;
    typedef CharT &reference;
    typedef CharT const &const_reference;

    typedef const_pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<const_iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // 24.4.2.1 Construction and assignment:

    nssv_constexpr basic_string_view() nssv_noexcept : data_(nssv_nullptr),
                                                       size_(0) {}

#if nssv_CPP11_OR_GREATER
    nssv_constexpr
    basic_string_view(basic_string_view const &other) nssv_noexcept = default;
#else
    nssv_constexpr
    basic_string_view(basic_string_view const &other) nssv_noexcept
        : data_(other.data_),
          size_(other.size_) {}
#endif

    nssv_constexpr basic_string_view(CharT const *s, size_type count)
        nssv_noexcept // non-standard noexcept
        : data_(s),
          size_(count) {}

    nssv_constexpr
    basic_string_view(CharT const *s) nssv_noexcept // non-standard noexcept
        : data_(s)
#if nssv_CPP17_OR_GREATER
        ,
          size_(Traits::length(s))
#elif nssv_CPP11_OR_GREATER
        ,
          size_(detail::length(s))
#else
        ,
          size_(Traits::length(s))
#endif
    {
    }

#if nssv_HAVE_NULLPTR
#if nssv_HAVE_IS_DELETE
    nssv_constexpr basic_string_view(std::nullptr_t) nssv_noexcept = delete;
#else
  private:
    nssv_constexpr basic_string_view(std::nullptr_t) nssv_noexcept;

  public:
#endif
#endif

    // Assignment:

#if nssv_CPP11_OR_GREATER
    nssv_constexpr14 basic_string_view &
    operator=(basic_string_view const &other) nssv_noexcept = default;
#else
    nssv_constexpr14 basic_string_view &
    operator=(basic_string_view const &other) nssv_noexcept {
      data_ = other.data_;
      size_ = other.size_;
      return *this;
    }
#endif

    // 24.4.2.2 Iterator support:

    nssv_constexpr const_iterator begin() const nssv_noexcept { return data_; }
    nssv_constexpr const_iterator end() const nssv_noexcept {
      return data_ + size_;
    }

    nssv_constexpr const_iterator cbegin() const nssv_noexcept {
      return begin();
    }
    nssv_constexpr const_iterator cend() const nssv_noexcept { return end(); }

    nssv_constexpr const_reverse_iterator rbegin() const nssv_noexcept {
      return const_reverse_iterator(end());
    }
    nssv_constexpr const_reverse_iterator rend() const nssv_noexcept {
      return const_reverse_iterator(begin());
    }

    nssv_constexpr const_reverse_iterator crbegin() const nssv_noexcept {
      return rbegin();
    }
    nssv_constexpr const_reverse_iterator crend() const nssv_noexcept {
      return rend();
    }

    // 24.4.2.3 Capacity:

    nssv_constexpr size_type size() const nssv_noexcept { return size_; }
    nssv_constexpr size_type length() const nssv_noexcept { return size_; }
    nssv_constexpr size_type max_size() const nssv_noexcept {
      return (std::numeric_limits<size_type>::max)();
    }

    // since C++20
    nssv_nodiscard nssv_constexpr bool empty() const nssv_noexcept {
      return 0 == size_;
    }

    // 24.4.2.4 Element access:

    nssv_constexpr const_reference operator[](size_type pos) const {
      return data_at(pos);
    }

    nssv_constexpr14 const_reference at(size_type pos) const {
#if nssv_CONFIG_NO_EXCEPTIONS
      assert(pos < size());
#else
      if (pos >= size()) {
        throw std::out_of_range("nonstd::string_view::at()");
      }
#endif
      return data_at(pos);
    }

    nssv_constexpr const_reference front() const { return data_at(0); }
    nssv_constexpr const_reference back() const { return data_at(size() - 1); }

    nssv_constexpr const_pointer data() const nssv_noexcept { return data_; }

    // 24.4.2.5 Modifiers:

    nssv_constexpr14 void remove_prefix(size_type n) {
      assert(n <= size());
      data_ += n;
      size_ -= n;
    }

    nssv_constexpr14 void remove_suffix(size_type n) {
      assert(n <= size());
      size_ -= n;
    }

    nssv_constexpr14 void swap(basic_string_view &other) nssv_noexcept {
      const basic_string_view tmp(other);
      other = *this;
      *this = tmp;
    }

    // 24.4.2.6 String operations:

    size_type copy(CharT *dest, size_type n, size_type pos = 0) const {
#if nssv_CONFIG_NO_EXCEPTIONS
      assert(pos <= size());
#else
      if (pos > size()) {
        throw std::out_of_range("nonstd::string_view::copy()");
      }
#endif
      const size_type rlen = (std::min)(n, size() - pos);

      (void)Traits::copy(dest, data() + pos, rlen);

      return rlen;
    }

    nssv_constexpr14 basic_string_view substr(size_type pos = 0,
                                              size_type n = npos) const {
#if nssv_CONFIG_NO_EXCEPTIONS
      assert(pos <= size());
#else
      if (pos > size()) {
        throw std::out_of_range("nonstd::string_view::substr()");
      }
#endif
      return basic_string_view(data() + pos, (std::min)(n, size() - pos));
    }

    // compare(), 6x:

    nssv_constexpr14 int
    compare(basic_string_view other) const nssv_noexcept // (1)
    {
#if nssv_CPP17_OR_GREATER
      if (const int result = Traits::compare(data(), other.data(),
                                             (std::min)(size(), other.size())))
#else
      if (const int result = detail::compare(data(), other.data(),
                                             (std::min)(size(), other.size())))
#endif
      {
        return result;
      }

      return size() == other.size() ? 0 : size() < other.size() ? -1 : 1;
    }

    nssv_constexpr int compare(size_type pos1, size_type n1,
                               basic_string_view other) const // (2)
    {
      return substr(pos1, n1).compare(other);
    }

    nssv_constexpr int compare(size_type pos1, size_type n1,
                               basic_string_view other, size_type pos2,
                               size_type n2) const // (3)
    {
      return substr(pos1, n1).compare(other.substr(pos2, n2));
    }

    nssv_constexpr int compare(CharT const *s) const // (4)
    {
      return compare(basic_string_view(s));
    }

    nssv_constexpr int compare(size_type pos1, size_type n1,
                               CharT const *s) const // (5)
    {
      return substr(pos1, n1).compare(basic_string_view(s));
    }

    nssv_constexpr int compare(size_type pos1, size_type n1, CharT const *s,
                               size_type n2) const // (6)
    {
      return substr(pos1, n1).compare(basic_string_view(s, n2));
    }

    // 24.4.2.7 Searching:

    // starts_with(), 3x, since C++20:

    nssv_constexpr bool
    starts_with(basic_string_view v) const nssv_noexcept // (1)
    {
      return size() >= v.size() && compare(0, v.size(), v) == 0;
    }

    nssv_constexpr bool starts_with(CharT c) const nssv_noexcept // (2)
    {
      return starts_with(basic_string_view(&c, 1));
    }

    nssv_constexpr bool starts_with(CharT const *s) const // (3)
    {
      return starts_with(basic_string_view(s));
    }

    // ends_with(), 3x, since C++20:

    nssv_constexpr bool
    ends_with(basic_string_view v) const nssv_noexcept // (1)
    {
      return size() >= v.size() && compare(size() - v.size(), npos, v) == 0;
    }

    nssv_constexpr bool ends_with(CharT c) const nssv_noexcept // (2)
    {
      return ends_with(basic_string_view(&c, 1));
    }

    nssv_constexpr bool ends_with(CharT const *s) const // (3)
    {
      return ends_with(basic_string_view(s));
    }

    // find(), 4x:

    nssv_constexpr14 size_type
    find(basic_string_view v, size_type pos = 0) const nssv_noexcept // (1)
    {
      return assert(v.size() == 0 || v.data() != nssv_nullptr),
             pos >= size() ? npos
                           : to_pos(
#if nssv_CPP11_OR_GREATER && !nssv_CPP17_OR_GREATER
                                 detail::search(substr(pos), v)
#else
                                 std::search(cbegin() + pos, cend(), v.cbegin(),
                                             v.cend(), Traits::eq)
#endif
                             );
    }

    nssv_constexpr size_type find(CharT c,
                                  size_type pos = 0) const nssv_noexcept // (2)
    {
      return find(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr size_type find(CharT const *s, size_type pos,
                                  size_type n) const // (3)
    {
      return find(basic_string_view(s, n), pos);
    }

    nssv_constexpr size_type find(CharT const *s,
                                  size_type pos = 0) const // (4)
    {
      return find(basic_string_view(s), pos);
    }

    // rfind(), 4x:

    nssv_constexpr14 size_type
    rfind(basic_string_view v, size_type pos = npos) const nssv_noexcept // (1)
    {
      if (size() < v.size()) {
        return npos;
      }

      if (v.empty()) {
        return (std::min)(size(), pos);
      }

      const_iterator last =
          cbegin() + (std::min)(size() - v.size(), pos) + v.size();
      const_iterator result =
          std::find_end(cbegin(), last, v.cbegin(), v.cend(), Traits::eq);

      return result != last ? size_type(result - cbegin()) : npos;
    }

    nssv_constexpr14 size_type
    rfind(CharT c, size_type pos = npos) const nssv_noexcept // (2)
    {
      return rfind(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr14 size_type rfind(CharT const *s, size_type pos,
                                     size_type n) const // (3)
    {
      return rfind(basic_string_view(s, n), pos);
    }

    nssv_constexpr14 size_type rfind(CharT const *s,
                                     size_type pos = npos) const // (4)
    {
      return rfind(basic_string_view(s), pos);
    }

    // find_first_of(), 4x:

    nssv_constexpr size_type find_first_of(
        basic_string_view v, size_type pos = 0) const nssv_noexcept // (1)
    {
      return pos >= size()
                 ? npos
                 : to_pos(std::find_first_of(cbegin() + pos, cend(), v.cbegin(),
                                             v.cend(), Traits::eq));
    }

    nssv_constexpr size_type
    find_first_of(CharT c, size_type pos = 0) const nssv_noexcept // (2)
    {
      return find_first_of(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr size_type find_first_of(CharT const *s, size_type pos,
                                           size_type n) const // (3)
    {
      return find_first_of(basic_string_view(s, n), pos);
    }

    nssv_constexpr size_type find_first_of(CharT const *s,
                                           size_type pos = 0) const // (4)
    {
      return find_first_of(basic_string_view(s), pos);
    }

    // find_last_of(), 4x:

    nssv_constexpr size_type find_last_of(
        basic_string_view v, size_type pos = npos) const nssv_noexcept // (1)
    {
      return empty()         ? npos
             : pos >= size() ? find_last_of(v, size() - 1)
                             : to_pos(std::find_first_of(
                                   const_reverse_iterator(cbegin() + pos + 1),
                                   crend(), v.cbegin(), v.cend(), Traits::eq));
    }

    nssv_constexpr size_type
    find_last_of(CharT c, size_type pos = npos) const nssv_noexcept // (2)
    {
      return find_last_of(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr size_type find_last_of(CharT const *s, size_type pos,
                                          size_type count) const // (3)
    {
      return find_last_of(basic_string_view(s, count), pos);
    }

    nssv_constexpr size_type find_last_of(CharT const *s,
                                          size_type pos = npos) const // (4)
    {
      return find_last_of(basic_string_view(s), pos);
    }

    // find_first_not_of(), 4x:

    nssv_constexpr size_type find_first_not_of(
        basic_string_view v, size_type pos = 0) const nssv_noexcept // (1)
    {
      return pos >= size()
                 ? npos
                 : to_pos(std::find_if(cbegin() + pos, cend(), not_in_view(v)));
    }

    nssv_constexpr size_type
    find_first_not_of(CharT c, size_type pos = 0) const nssv_noexcept // (2)
    {
      return find_first_not_of(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr size_type find_first_not_of(CharT const *s, size_type pos,
                                               size_type count) const // (3)
    {
      return find_first_not_of(basic_string_view(s, count), pos);
    }

    nssv_constexpr size_type find_first_not_of(CharT const *s,
                                               size_type pos = 0) const // (4)
    {
      return find_first_not_of(basic_string_view(s), pos);
    }

    // find_last_not_of(), 4x:

    nssv_constexpr size_type find_last_not_of(
        basic_string_view v, size_type pos = npos) const nssv_noexcept // (1)
    {
      return empty()         ? npos
             : pos >= size() ? find_last_not_of(v, size() - 1)
                             : to_pos(std::find_if(
                                   const_reverse_iterator(cbegin() + pos + 1),
                                   crend(), not_in_view(v)));
    }

    nssv_constexpr size_type
    find_last_not_of(CharT c, size_type pos = npos) const nssv_noexcept // (2)
    {
      return find_last_not_of(basic_string_view(&c, 1), pos);
    }

    nssv_constexpr size_type find_last_not_of(CharT const *s, size_type pos,
                                              size_type count) const // (3)
    {
      return find_last_not_of(basic_string_view(s, count), pos);
    }

    nssv_constexpr size_type find_last_not_of(CharT const *s,
                                              size_type pos = npos) const // (4)
    {
      return find_last_not_of(basic_string_view(s), pos);
    }

    // Constants:

#if nssv_CPP17_OR_GREATER
    static nssv_constexpr size_type npos = size_type(-1);
#elif nssv_CPP11_OR_GREATER
    enum : size_type { npos = size_type(-1) };
#else
    enum { npos = size_type(-1) };
#endif

  private:
    struct not_in_view {
      const basic_string_view v;

      nssv_constexpr explicit not_in_view(basic_string_view v_) : v(v_) {}

      nssv_constexpr bool operator()(CharT c) const {
        return npos == v.find_first_of(c);
      }
    };

    nssv_constexpr size_type to_pos(const_iterator it) const {
      return it == cend() ? npos : size_type(it - cbegin());
    }

    nssv_constexpr size_type to_pos(const_reverse_iterator it) const {
      return it == crend() ? npos : size_type(crend() - it - 1);
    }

    nssv_constexpr const_reference data_at(size_type pos) const {
#if nssv_BETWEEN(nssv_COMPILER_GNUC_VERSION, 1, 500)
      return data_[pos];
#else
      return assert(pos < size()), data_[pos];
#endif
    }

  private:
    const_pointer data_;
    size_type size_;

  public:
#if nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS

    template <class Allocator>
    basic_string_view(std::basic_string<CharT, Traits, Allocator> const &s)
        nssv_noexcept : data_(s.data()),
                        size_(s.size()) {}

#if nssv_HAVE_EXPLICIT_CONVERSION

    template <class Allocator>
    explicit operator std::basic_string<CharT, Traits, Allocator>() const {
      return to_string(Allocator());
    }

#endif // nssv_HAVE_EXPLICIT_CONVERSION

#if nssv_CPP11_OR_GREATER

    template <class Allocator = std::allocator<CharT>>
    std::basic_string<CharT, Traits, Allocator>
    to_string(Allocator const &a = Allocator()) const {
      return std::basic_string<CharT, Traits, Allocator>(begin(), end(), a);
    }

#else

    std::basic_string<CharT, Traits> to_string() const {
      return std::basic_string<CharT, Traits>(begin(), end());
    }

    template <class Allocator>
    std::basic_string<CharT, Traits, Allocator>
    to_string(Allocator const &a) const {
      return std::basic_string<CharT, Traits, Allocator>(begin(), end(), a);
    }

#endif // nssv_CPP11_OR_GREATER

#endif // nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
  };

  //
  // Non-member functions:
  //

  // 24.4.3 Non-member comparison functions:
  // lexicographically compare two string views (function template):

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator==(basic_string_view<CharT, Traits> lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator!=(basic_string_view<CharT, Traits> lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<(basic_string_view<CharT, Traits> lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) < 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<=(basic_string_view<CharT, Traits> lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) <= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>(basic_string_view<CharT, Traits> lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) > 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>=(basic_string_view<CharT, Traits> lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) >= 0;
  }

  // Let S be basic_string_view<CharT, Traits>, and sv be an instance of S.
  // Implementations shall provide sufficient additional overloads marked
  // constexpr and noexcept so that an object t with an implicit conversion
  // to S can be compared according to Table 67.

#if !nssv_CPP11_OR_GREATER || nssv_BETWEEN(nssv_COMPILER_MSVC_VERSION, 100, 141)

  // accommodate for older compilers:

  // ==

  template <class CharT, class Traits>
  nssv_constexpr bool operator==(basic_string_view<CharT, Traits> lhs,
                                 CharT const *rhs) nssv_noexcept {
    return lhs.size() == detail::length(rhs) && lhs.compare(rhs) == 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator==(CharT const *lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return detail::length(lhs) == rhs.size() && rhs.compare(lhs) == 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator==(basic_string_view<CharT, Traits> lhs,
             std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator==(std::basic_string<CharT, Traits> rhs,
             basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }

  // !=

  template <class CharT, class Traits>
  nssv_constexpr bool operator!=(basic_string_view<CharT, Traits> lhs,
                                 CharT const *rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator!=(CharT const *lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator!=(basic_string_view<CharT, Traits> lhs,
             std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator!=(std::basic_string<CharT, Traits> rhs,
             basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  // <

  template <class CharT, class Traits>
  nssv_constexpr bool operator<(basic_string_view<CharT, Traits> lhs,
                                CharT const *rhs) nssv_noexcept {
    return lhs.compare(rhs) < 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<(CharT const *lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return rhs.compare(lhs) > 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<(basic_string_view<CharT, Traits> lhs,
            std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) < 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<(std::basic_string<CharT, Traits> rhs,
            basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return rhs.compare(lhs) > 0;
  }

  // <=

  template <class CharT, class Traits>
  nssv_constexpr bool operator<=(basic_string_view<CharT, Traits> lhs,
                                 CharT const *rhs) nssv_noexcept {
    return lhs.compare(rhs) <= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<=(CharT const *lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return rhs.compare(lhs) >= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<=(basic_string_view<CharT, Traits> lhs,
             std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) <= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator<=(std::basic_string<CharT, Traits> rhs,
             basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return rhs.compare(lhs) >= 0;
  }

  // >

  template <class CharT, class Traits>
  nssv_constexpr bool operator>(basic_string_view<CharT, Traits> lhs,
                                CharT const *rhs) nssv_noexcept {
    return lhs.compare(rhs) > 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>(CharT const *lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return rhs.compare(lhs) < 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>(basic_string_view<CharT, Traits> lhs,
            std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) > 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>(std::basic_string<CharT, Traits> rhs,
            basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return rhs.compare(lhs) < 0;
  }

  // >=

  template <class CharT, class Traits>
  nssv_constexpr bool operator>=(basic_string_view<CharT, Traits> lhs,
                                 CharT const *rhs) nssv_noexcept {
    return lhs.compare(rhs) >= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>=(CharT const *lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return rhs.compare(lhs) <= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>=(basic_string_view<CharT, Traits> lhs,
             std::basic_string<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) >= 0;
  }

  template <class CharT, class Traits>
  nssv_constexpr bool
  operator>=(std::basic_string<CharT, Traits> rhs,
             basic_string_view<CharT, Traits> lhs) nssv_noexcept {
    return rhs.compare(lhs) <= 0;
  }

#else // newer compilers:

#define nssv_BASIC_STRING_VIEW_I(T, U)                                         \
  typename std::decay<basic_string_view<T, U>>::type

#if defined(_MSC_VER) // issue 40
#define nssv_MSVC_ORDER(x) , int = x
#else
#define nssv_MSVC_ORDER(x) /*, int=x*/
#endif

  // ==

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator==(basic_string_view<CharT, Traits> lhs,
                                 nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                     rhs) nssv_noexcept {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator==(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }

  // !=

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator!=(basic_string_view<CharT, Traits> lhs,
                                 nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                     rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator!=(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return !(lhs == rhs);
  }

  // <

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator<(basic_string_view<CharT, Traits> lhs,
                                nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                    rhs) nssv_noexcept {
    return lhs.compare(rhs) < 0;
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator<(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) < 0;
  }

  // <=

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator<=(basic_string_view<CharT, Traits> lhs,
                                 nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                     rhs) nssv_noexcept {
    return lhs.compare(rhs) <= 0;
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator<=(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) <= 0;
  }

  // >

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator>(basic_string_view<CharT, Traits> lhs,
                                nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                    rhs) nssv_noexcept {
    return lhs.compare(rhs) > 0;
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator>(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
            basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) > 0;
  }

  // >=

  template <class CharT, class Traits nssv_MSVC_ORDER(1)>
  nssv_constexpr bool operator>=(basic_string_view<CharT, Traits> lhs,
                                 nssv_BASIC_STRING_VIEW_I(CharT, Traits)
                                     rhs) nssv_noexcept {
    return lhs.compare(rhs) >= 0;
  }

  template <class CharT, class Traits nssv_MSVC_ORDER(2)>
  nssv_constexpr bool
  operator>=(nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
             basic_string_view<CharT, Traits> rhs) nssv_noexcept {
    return lhs.compare(rhs) >= 0;
  }

#undef nssv_MSVC_ORDER
#undef nssv_BASIC_STRING_VIEW_I

#endif // compiler-dependent approach to comparisons

  // 24.4.4 Inserters and extractors:

#if !nssv_CONFIG_NO_STREAM_INSERTION

  namespace detail {

  template <class Stream> void write_padding(Stream &os, std::streamsize n) {
    for (std::streamsize i = 0; i < n; ++i)
      os.rdbuf()->sputc(os.fill());
  }

  template <class Stream, class View>
  Stream &write_to_stream(Stream &os, View const &sv) {
    typename Stream::sentry sentry(os);

    if (!sentry)
      return os;

    const std::streamsize length = static_cast<std::streamsize>(sv.length());

    // Whether, and how, to pad:
    const bool pad = (length < os.width());
    const bool left_pad = pad && (os.flags() & std::ios_base::adjustfield) ==
                                     std::ios_base::right;

    if (left_pad)
      write_padding(os, os.width() - length);

    // Write span characters:
    os.rdbuf()->sputn(sv.begin(), length);

    if (pad && !left_pad)
      write_padding(os, os.width() - length);

    // Reset output stream width:
    os.width(0);

    return os;
  }

  } // namespace detail

  template <class CharT, class Traits>
  std::basic_ostream<CharT, Traits> &
  operator<<(std::basic_ostream<CharT, Traits> &os,
             basic_string_view<CharT, Traits> sv) {
    return detail::write_to_stream(os, sv);
  }

#endif // nssv_CONFIG_NO_STREAM_INSERTION

  // Several typedefs for common character types are provided:

  typedef basic_string_view<char> string_view;
  typedef basic_string_view<wchar_t> wstring_view;
#if nssv_HAVE_WCHAR16_T
  typedef basic_string_view<char16_t> u16string_view;
  typedef basic_string_view<char32_t> u32string_view;
#endif

  } // namespace sv_lite
} // namespace nonstd::sv_lite

//
// 24.4.6 Suffix for basic_string_view literals:
//

#if nssv_HAVE_USER_DEFINED_LITERALS

namespace nonstd {
nssv_inline_ns namespace literals {
  nssv_inline_ns namespace string_view_literals {

#if nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

    nssv_constexpr nonstd::sv_lite::string_view operator"" sv(
        const char *str, size_t len) nssv_noexcept // (1)
    {
      return nonstd::sv_lite::string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::u16string_view operator"" sv(
        const char16_t *str, size_t len) nssv_noexcept // (2)
    {
      return nonstd::sv_lite::u16string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::u32string_view operator"" sv(
        const char32_t *str, size_t len) nssv_noexcept // (3)
    {
      return nonstd::sv_lite::u32string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::wstring_view operator"" sv(
        const wchar_t *str, size_t len) nssv_noexcept // (4)
    {
      return nonstd::sv_lite::wstring_view{str, len};
    }

#endif // nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

#if nssv_CONFIG_USR_SV_OPERATOR

    nssv_constexpr nonstd::sv_lite::string_view operator"" _sv(
        const char *str, size_t len) nssv_noexcept // (1)
    {
      return nonstd::sv_lite::string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::u16string_view operator"" _sv(
        const char16_t *str, size_t len) nssv_noexcept // (2)
    {
      return nonstd::sv_lite::u16string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::u32string_view operator"" _sv(
        const char32_t *str, size_t len) nssv_noexcept // (3)
    {
      return nonstd::sv_lite::u32string_view{str, len};
    }

    nssv_constexpr nonstd::sv_lite::wstring_view operator"" _sv(
        const wchar_t *str, size_t len) nssv_noexcept // (4)
    {
      return nonstd::sv_lite::wstring_view{str, len};
    }

#endif // nssv_CONFIG_USR_SV_OPERATOR
  }
}
} // namespace nonstd

#endif

//
// Extensions for std::string:
//

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {
namespace sv_lite {

// Exclude MSVC 14 (19.00): it yields ambiguous to_string():

#if nssv_CPP11_OR_GREATER && nssv_COMPILER_MSVC_VERSION != 140

template <class CharT, class Traits, class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator>
to_string(basic_string_view<CharT, Traits> v,
          Allocator const &a = Allocator()) {
  return std::basic_string<CharT, Traits, Allocator>(v.begin(), v.end(), a);
}

#else

template <class CharT, class Traits>
std::basic_string<CharT, Traits> to_string(basic_string_view<CharT, Traits> v) {
  return std::basic_string<CharT, Traits>(v.begin(), v.end());
}

template <class CharT, class Traits, class Allocator>
std::basic_string<CharT, Traits, Allocator>
to_string(basic_string_view<CharT, Traits> v, Allocator const &a) {
  return std::basic_string<CharT, Traits, Allocator>(v.begin(), v.end(), a);
}

#endif // nssv_CPP11_OR_GREATER

template <class CharT, class Traits, class Allocator>
basic_string_view<CharT, Traits>
to_string_view(std::basic_string<CharT, Traits, Allocator> const &s) {
  return basic_string_view<CharT, Traits>(s.data(), s.size());
}

} // namespace sv_lite
} // namespace nonstd

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

//
// make types and algorithms available in namespace nonstd:
//

namespace nonstd {

using sv_lite::basic_string_view;
using sv_lite::string_view;
using sv_lite::wstring_view;

#if nssv_HAVE_WCHAR16_T
using sv_lite::u16string_view;
#endif
#if nssv_HAVE_WCHAR32_T
using sv_lite::u32string_view;
#endif

// literal "sv"

using sv_lite::operator==;
using sv_lite::operator!=;
using sv_lite::operator<;
using sv_lite::operator<=;
using sv_lite::operator>;
using sv_lite::operator>=;

#if !nssv_CONFIG_NO_STREAM_INSERTION
using sv_lite::operator<<;
#endif

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
using sv_lite::to_string;
using sv_lite::to_string_view;
#endif

} // namespace nonstd

// 24.4.5 Hash support (C++11):

// Note: The hash value of a string view object is equal to the hash value of
// the corresponding string object.

#if nssv_HAVE_STD_HASH

#include <functional>

namespace std {

template <> struct hash<nonstd::string_view> {
public:
  std::size_t operator()(nonstd::string_view v) const nssv_noexcept {
    return std::hash<std::string>()(std::string(v.data(), v.size()));
  }
};

template <> struct hash<nonstd::wstring_view> {
public:
  std::size_t operator()(nonstd::wstring_view v) const nssv_noexcept {
    return std::hash<std::wstring>()(std::wstring(v.data(), v.size()));
  }
};

template <> struct hash<nonstd::u16string_view> {
public:
  std::size_t operator()(nonstd::u16string_view v) const nssv_noexcept {
    return std::hash<std::u16string>()(std::u16string(v.data(), v.size()));
  }
};

template <> struct hash<nonstd::u32string_view> {
public:
  std::size_t operator()(nonstd::u32string_view v) const nssv_noexcept {
    return std::hash<std::u32string>()(std::u32string(v.data(), v.size()));
  }
};

} // namespace std

#endif // nssv_HAVE_STD_HASH

nssv_RESTORE_WARNINGS()

#endif // nssv_HAVE_STD_STRING_VIEW
#endif // NONSTD_SV_LITE_H_INCLUDED