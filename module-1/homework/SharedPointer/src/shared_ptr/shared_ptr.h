#pragma once

#include "../control/control.h"

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    constexpr SharedPtr() noexcept = default;
    ~SharedPtr();
    template <typename Y>
    explicit SharedPtr(Y* p) : control_(new ControlBlock<T>(p)) {
        control_->AddStrongPtr();
    };

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept;

    SharedPtr(const SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;

    SharedPtr& operator=(const SharedPtr& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept;

    SharedPtr& operator=(SharedPtr&& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept;

    SharedPtr(const WeakPtr<T>& other) noexcept;

    // Modifiers
    void Reset() noexcept;

    template <typename Y>
    void Reset(Y* p) noexcept;

    template <typename Y, typename Deleter>
    void Reset(Y* p, Deleter deleter) noexcept;

    void Swap(SharedPtr& other) noexcept;

    // Observers
    T* Get() const noexcept;
    int64_t UseCount() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    T& operator[](std::ptrdiff_t idx) const;
    explicit operator bool() const noexcept {
        return control_ && !control_->IsEmpty();
    };

    template <typename U>
    friend class WeakPtr;

private:
    ControlBlock<T>* control_ = nullptr;
};

template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (control_) {
        control_->DelShared();
    }
}

// MakeShared
// Your code goes here...
// MakeShared

// SharedPtr
// Your code goes here...
// SharedPtr

// WeakPtr
template <typename T>
class WeakPtr {

public:
    // Special-member functions
    constexpr WeakPtr() noexcept = default;
    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other);
    WeakPtr(const WeakPtr& other) noexcept;
    WeakPtr(WeakPtr&& other) noexcept;
    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other);
    WeakPtr& operator=(const WeakPtr& other) noexcept;
    WeakPtr& operator=(WeakPtr&& other) noexcept;

    ~WeakPtr();

    // Modifiers
    void Reset() noexcept;
    void Swap(WeakPtr<T>& other) noexcept;

    // Observers
    bool Expired() const noexcept;
    SharedPtr<T> Lock() const noexcept;

    template <typename U>
    friend class SharedPtr;

public:
    ControlBlock<T>* control_;
};

template <typename T>
T &SharedPtr<T>::operator*() const noexcept {
    return *(control_->GetPtr());
}

template <typename T>
T *SharedPtr<T>::operator->() const noexcept {
    return control_->GetPtr();
}

template <typename T>
int64_t SharedPtr<T>::UseCount() const noexcept {
    return control_->GetStrongCount();
}

template <typename T>
void SharedPtr<T>::Reset() noexcept {
    control_ = nullptr;
}

template <typename T>
template <typename Y, typename Deleter>
SharedPtr<T>::SharedPtr(Y* p, Deleter deleter) noexcept :
    control_(new ControlBlock<T, Deleter>(p, deleter)) {
    control_->AddStrongPtr();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept {
    control_ = other.control_;
    control_->AddStrongPtr();
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& r) noexcept {
    if (&r == this) {
        return *this;
    }

    if (control_) {
        control_->DelShared();
    }

    control_ = r.control_;
    control_->AddStrongPtr();
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& r) noexcept {
    if (&r == this) {
        return *this;
    }

    if (control_) {
        control_->DelShared();
    }

    control_ = new ControlBlock<T>();
    control_->SetPtr(r.control_->GetPtr());
    control_->SetValues(r.control_->GetStrongCount(), r.control_->GetWeakCount());
    r.control_->SetValues(0, 0);
    r.control_->SetPtr(nullptr);
    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept {
    control_ = new ControlBlock<T>();
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());
    control_->SetPtr(other.control_->GetPtr());
    other.control_->SetPtr(nullptr);
    other.control_->SetValues(0, 0);
}

template <typename T>
void SharedPtr<T>::Swap(SharedPtr& other) noexcept {
    swap(control_, other.control_);
}

template <typename T>
T *SharedPtr<T>::Get() const noexcept {
    return control_->GetPtr();
}

template <typename T>
template <typename Y>
void SharedPtr<T>::Reset(Y* p) noexcept {
    SharedPtr<T>(p).Swap(*this);
}

template <typename T>
template <typename Y, typename Deleter>
void SharedPtr<T>::Reset(Y* p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).Swap(*this);
}

template <typename T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) noexcept {
    control_ = other.control_;

    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());
    control_->AddStrongPtr();
}

template <typename T>
template <typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y>& other) {
    control_ = new ControlBlock<T>();
    control_->SetPtr(other.control_->GetPtr());
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());

    control_->AddWeakPtr();
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept {
    control_ = new ControlBlock<T>();
    control_->SetPtr(other.control_->GetPtr());
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());

    control_->AddWeakPtr();
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept {
    control_ = new ControlBlock<T>();
    control_->SetPtr(other.control_->GetPtr());
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());

    other.control_.SetPtr(nullptr);
    other.control_.SetValues(0, 0);
}

template <typename T>
template <typename Y>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<Y>& other) {
    control_ = other.control_;

    control_->AddWeakPtr();
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (control_) {
        control_->DelWeak();
    }

    control_->SetPtr(other.control_->GetPtr());
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());

    control_->AddWeakPtr();
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (control_) {
        control_->DelWeak();
    }

    control_ = new ControlBlock<T>();
    control_->SetPtr(other.control_->GetPtr());
    control_->SetValues(other.control_->GetStrongCount(), other.control_->GetWeakCount());

    other.control_.SetPtr(nullptr);
    other.control_.SetValues(0, 0);
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    control_->DelWeak();
}

template <typename T>
void WeakPtr<T>::Reset() noexcept {
    if (control_) {
        control_->SetPtr(nullptr);
        control_->SetValues(0, 0);
    }
}

template <typename T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) noexcept {
    auto tmp = other.control_;
    other.control_ = control_;
    control_ = tmp;
}

template <typename T>
bool WeakPtr<T>::Expired() const noexcept {
    if (control_) {
        return control_->GetStrongCount() == 0;
    }
    return true;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::Lock() const noexcept {
    if (Expired()) {
        return SharedPtr<T>();
    } else {
        return SharedPtr<T>(*this);
    }
}
