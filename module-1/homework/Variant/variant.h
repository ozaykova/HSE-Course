#include <cstdlib>
#include <utility>

#pragma once

namespace task {
template <typename Head, typename... Tail>
struct TypeList {
    using head = Head;
    using tail = TypeList<Tail...>;
};

template <unsigned int index, class TList>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<0, TypeList<Head, Tail...>> {
    using result = Head;
};

template <unsigned int index, typename Head, typename... Tail>
struct TypeAt<index, TypeList<Head, Tail...>> {
    using result = typename TypeAt<index - 1, TypeList<Tail...>>::result;
};

template <size_t Index>
struct InPlaceIndex {
    explicit InPlaceIndex() = default;
};

template <size_t Index>
constexpr InPlaceIndex<Index> kInPlaceIndex{};

template <size_t Index, typename... Types>
union Union;

template <size_t Index>
union Union<Index> {};

template <size_t Index, typename T, typename... Types>
union Union<Index, T, Types...> {
public:
    T head;
    Union<Index + 1, Types...> tail;

    Union() : tail() {
        if (!std::is_trivially_constructible_v<T>) {
            new (&head) T();
        }
    }

    ~Union() {
        if (!std::is_trivially_destructible_v<T>) {
            head.~T();
        }
    }
};

struct AssignUnion {
    template <size_t Index, typename U, size_t UnionIndex, typename Head, typename... Tail>
    static void SetVal(U&& value, InPlaceIndex<0>, Union<UnionIndex, Head, Tail...>& u) {
        u.head = value;
    }

    template <size_t Index, typename U, size_t UnionIndex, typename Head, typename... Tail>
    static void SetVal(U&& value, InPlaceIndex<Index>, Union<UnionIndex, Head, Tail...>& u) {
        AssignUnion::SetVal<Index - 1>(std::forward<U>(value), kInPlaceIndex<Index - 1>, u.tail);
    }
};

struct AccessUnion {
    template <typename U>
    static constexpr auto&& GetVal(U&& value, InPlaceIndex<0>) {
        return std::forward<U>(value).head;
    }

    template <typename U, size_t Index>
    static constexpr auto&& GetVal(U&& value, InPlaceIndex<Index>) {
        return GetVal(std::forward<U>(value).tail, kInPlaceIndex<Index - 1>);
    }
};

struct AccessVariant {
    template <size_t Index, typename T>
    static constexpr auto&& GetVal(T&& value) {
        return AccessUnion::GetVal(std::forward<T>(value).data_, kInPlaceIndex<Index>);
    }
};

const static size_t kNotFounded = -1;
const static size_t kAmbiguity = kNotFounded - 1;

template <size_t SizeOfFounded>
constexpr size_t FindCondition(size_t i, size_t res, const bool (&founded)[SizeOfFounded],
                               const bool (&convertible)[SizeOfFounded]) {
    if (res == kNotFounded && (founded[i] || convertible[i])) {
        return i;
    }
    if (res == kAmbiguity) {
        return res;
    }
    if (founded[i]) {
        if (!founded[res] && convertible[res]) {
            return i;
        }
        return kAmbiguity;
    }

    return res;
}

template <size_t SizeOfFounded>
constexpr size_t FindPos(size_t current_position, const bool (&founded)[SizeOfFounded],
                         const bool (&convertible)[SizeOfFounded]) {
    if (current_position != SizeOfFounded) {
        return FindCondition(current_position, FindPos(current_position + 1, founded, convertible),
                             founded, convertible);
    }
    return kNotFounded;
}

template <typename TargetType, typename... Types>
struct FindExactlyOneChecked {
    constexpr static bool kFounded[sizeof...(Types)] = {std::is_same<TargetType, Types>::value...};
    constexpr static bool kFoundedConvertible[sizeof...(Types)] = {std::is_convertible<TargetType, Types>::value...};
    constexpr static size_t kValue = FindPos(0, kFounded, kFoundedConvertible);
};

template <typename T>
struct FindExactlyOneChecked<T> {
    static_assert(!std::is_same<T, T>::value, "Type not in empty type list");
};

template <typename TargetType, typename... Types>
struct FindExactlyOneType : public FindExactlyOneChecked<TargetType, Types...> {};

template <typename... Types>
class Variant;

template <size_t Idx, typename T>
struct VariantAlternative;

template <size_t Idx, typename T>
using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

template <size_t Idx, typename... Types>
struct VariantAlternative<Idx, Variant<Types...>> {
  using type = typename TypeAt<Idx, TypeList<Types...>>::result;
};

template <typename... Types>
class Variant {
public:
    // Special member functions
    constexpr Variant() noexcept {};

    template <typename T, size_t Position = FindExactlyOneType<T, Types...>::kValue>
    Variant& operator=(T&& t) noexcept {
        AssignUnion::SetVal<Position>(std::forward<T>(t), kInPlaceIndex<Position>, data_);
        return *this;
    }

private:
    Union<0, Types...> data_;
    friend AccessVariant;
};

template <size_t Idx, typename T>
auto&& GenericGet(T&& v) {
    return AccessVariant::GetVal<Idx>(std::forward<T>(v));
}

// Non-member functions
template <size_t I, typename... Types>
constexpr const variant_alternative_t<I, Variant<Types...>>& Get(Variant<Types...>& v) {
    return GenericGet<I>(v);
}

template <size_t I, typename... Types>
constexpr variant_alternative_t<I, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return GenericGet<I>(v);
}

template <typename T, typename... Types>
constexpr const T& Get(Variant<Types...>& v) {
    return GenericGet<FindExactlyOneType<T, Types...>::kValue>(v);
}

template <typename T, typename... Types>
constexpr T&& Get(Variant<Types...>&& v) {
    return GenericGet<FindExactlyOneType<T, Types...>::kValue>(v);
}

};  // namespace task
