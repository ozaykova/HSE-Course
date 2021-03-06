#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {
    explicit constexpr NullOpt(int) {
    }
};

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace = InPlace();

template <typename T, bool = std::is_trivially_destructible_v<T>>
class DestructHelper;

template <typename T>
class DestructHelper<T, false> {
public:
    explicit constexpr DestructHelper() noexcept : null_(), engaged_(false) {
    }

    explicit constexpr DestructHelper(NullOpt) noexcept : null_(), engaged_(false) {
    }

    template <typename... Args>
    explicit constexpr DestructHelper(InPlace, Args&&... args)
        : val_(std::forward<Args>(args)...), engaged_(true) {
    }

    template <typename U = T>
    explicit constexpr DestructHelper(U&& value) : val_(std::forward<U>(value)), engaged_(true) {
    }

    ~DestructHelper() {
        if (engaged_) {
            val_.~T();
        }
    }

protected:
    union {
        char null_;
        T val_;
    };

    bool engaged_;

    void Reset() {
        if (engaged_) {
            engaged_ = false;
        }
    }

    template <typename U = T>
    void Set(U&& value) {
        val_ = std::forward<U>(value);
        engaged_ = true;
    }
};

template <typename T>
class DestructHelper<T, true> {
public:
    explicit constexpr DestructHelper() noexcept : null_(), engaged_(false) {
    }

    explicit constexpr DestructHelper(NullOpt) noexcept : null_(), engaged_(false) {
    }

    template <typename... Args>
    explicit constexpr DestructHelper(InPlace, Args&&... args)
        : val_(std::forward<Args>(args)...), engaged_(true) {
    }

    template <typename U = T>
    explicit constexpr DestructHelper(U&& value) : val_(std::forward<U>(value)), engaged_(true) {
    }

protected:
    union {
        char null_;
        T val_;
    };

    bool engaged_;

    void Reset() {
        if (engaged_) {
            engaged_ = false;
        }
    }

    template <typename U = T>
    void Set(U&& value) {
        val_ = std::forward<U>(value);
        engaged_ = true;
    }
};

template <typename T>
class Optional : public DestructHelper<T> {
public:
    using value_type = T;
    using base = DestructHelper<T>;

    constexpr Optional() noexcept = default;

    template <typename U = value_type>
    explicit constexpr Optional(U&& value) : base(std::forward<U>(value)) {
    }

    explicit constexpr Optional(NullOpt) noexcept {
    }

    template <typename... Args>
    explicit constexpr Optional(InPlace, Args&&... args)
        : base(kInPlace, std::forward<Args>(args)...) {
    }

    Optional& operator=(NullOpt) noexcept {
        base::Reset();
        return *this;
    }

    template <typename U = T>
    Optional& operator=(U&& value) {
        base::Set(std::forward<U>(value));
        return *this;
    };

    void Reset() noexcept {
        base::Reset();
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) const& {
        if (base::engaged_) {
            return base::val_;
        }
        return default_value;
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) && {
        if (base::engaged_) {
            return base::val_;
        }
        return default_value;
    }

    constexpr bool HasValue() const noexcept;

    explicit constexpr operator bool() const noexcept;

    constexpr std::add_pointer_t<const value_type> operator->() const;

    constexpr std::add_pointer_t<value_type> operator->();

    constexpr const value_type& operator*() const&;

    constexpr value_type& operator*() &;

    constexpr const value_type&& operator*() const&&;

    constexpr value_type&& operator*() &&;
};

template <typename T>
constexpr bool Optional<T>::HasValue() const noexcept {
    return base::engaged_;
}

template <typename T>
constexpr Optional<T>::operator bool() const noexcept {
    return base::engaged_;
}

template <typename T>
constexpr std::add_pointer_t<const T> Optional<T>::operator->() const {
    return base::val_;
}

template <typename T>
constexpr std::add_pointer_t<T> Optional<T>::operator->() {
    return &(base::val_);
}

template <typename T>
constexpr const T& Optional<T>::operator*() const& {
    return base::val_;
}

template <typename T>
constexpr T& Optional<T>::operator*() & {
    return base::val_;
}

template <typename T>
constexpr const T&& Optional<T>::operator*() const&& {
    return std::move(base::val_);
}

template <typename T>
constexpr T&& Optional<T>::operator*() && {
    return std::move(base::val_);
}
}  // namespace task
