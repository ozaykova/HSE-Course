#pragma once

#include <type_traits>

#include "is_copy_constructible.h"
#include "utility.h"

//<IsConstructible, is_reference, T, Args...>
template <bool, bool, typename T, typename... Args>
struct LibCppIsNoThrowConstructible;

// LibCppIsNoThrowConstructible - partial specializations
// Your code goes here
// LibCppIsNoThrowConstructible - partial specializations

template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, false, T, Args...>
        : public std::integral_constant<bool, noexcept(T(Declval<Args>()...))> {};

template <typename T>
void ImplicitConversionTo(T) noexcept {
}

template <typename T, typename Arg>
struct LibCppIsNoThrowConstructible<true, true, T, Arg>
        : public std::integral_constant<bool, noexcept(ImplicitConversionTo<T>(Declval<Arg>()))> {};

template <typename T, bool IsReference, typename... Args>
struct LibCppIsNoThrowConstructible<false, IsReference, T, Args...> : public std::false_type {};

template <typename T, typename... Args>
struct IsNoThrowConstructible : LibCppIsNoThrowConstructible<IsConstructible<T, Args...>::value,
        std::is_reference_v<T>, T, Args...> {};

template <typename T, std::size_t N>
struct IsNoThrowConstructible<T[N]> : LibCppIsNoThrowConstructible<IsConstructible<T>::value, std::is_reference_v<T>, T> {
};

template <typename T>
struct IsNoThrowMoveConstructible : public IsNoThrowConstructible<T, typename AddRvalueReference<T>::type> {};