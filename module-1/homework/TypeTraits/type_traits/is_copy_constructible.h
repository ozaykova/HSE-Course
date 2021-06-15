#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using rawDerived = Uncvref<Derived>;
    using rawBase = Uncvref<Base>;

    static const bool result = !std::is_same_v<rawBase, rawDerived> &&
                               std::is_base_of_v<rawBase, rawDerived> &&
                               !LibCppIsConstructible<rawDerived, rawBase>::type::value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {
    // Your code goes here
};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using raw_to = Uncvref<RefTo>;
    using raw_from = Uncvref<RefFrom>;

    static const bool result = !std::is_function_v<raw_to>
        && (std::is_same_v<raw_from, raw_to> || std::is_base_of_v<raw_to, raw_from>);
};

template <typename T>
T&& Declval(int);

template <typename T>
T Declval(int64_t);

template <typename T>
decltype(Declval<T>(0)) Declval() noexcept;

struct IsConstructibleHelper {
    template <typename T, typename... Args, typename = decltype(T(Declval<Args>()...))>
    static std::true_type IsNray(int);

    template <typename T, typename...>
    static std::false_type IsNray(...);

    template <typename T, typename A, typename = decltype(::new T(Declval<A>()))>
    static std::is_destructible<T> IsUnary(int);

    template <typename, typename>
    static std::false_type IsUnary(...);

    template <typename T>
    static void Wrap(T);

    template <typename To, typename From, typename = decltype(Wrap<To>(Declval<From>()))>
    static std::true_type TryCast(int);

    template <typename To, typename From, typename = decltype(static_cast<To>(Declval<From>()))>
    static std::integral_constant<bool, !IsInvalidBaseToDerivedCast<To, From>::kValue &&
                                        !IsInvalidLvalueToRvalueCast<To, From>::kValue>
    TryCast(int);

    template <typename, typename>
    static std::false_type TryCast(...);
};

// LibCppIsConstructible - partial specializations
// Your code goes here
// LibCppIsConstructible - partial specializations

template <typename T, typename... Args>
struct LibCppIsConstructible {
    static_assert(sizeof...(Args) > 1, "Wrong specialization");
    using type = decltype(IsConstructibleHelper::IsNray<T, Args...>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T, A> : public decltype(IsConstructibleHelper::IsUnary<T, A>(0)) {};

template <typename T, typename A>
struct LibCppIsConstructible<T&, A> : public decltype(IsConstructibleHelper::TryCast<T&, A>(0)) {};

template <typename T, typename A>
struct LibCppIsConstructible<T&&, A> : public decltype(IsConstructibleHelper::TryCast<T&&, A>(0)) {
};

template <typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::type {};

template <typename T>
struct IsCopyConstructible
    : public IsConstructible<T, typename AddLvalueReference<typename AddConst<T>::type>::type> {};