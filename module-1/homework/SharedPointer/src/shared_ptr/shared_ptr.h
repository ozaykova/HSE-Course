#pragma once

#include "../control/control.h"

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = T;    // Your code goes here...

    constexpr SharedPtr() noexcept = default;
    ~SharedPtr();
    template <typename Y>
    explicit SharedPtr(Y* p) : Control(new ControlBlock<T>(p)) {Control->AddStrongPtr();};

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
    element_type& operator[](std::ptrdiff_t idx) const;
    explicit operator bool() const noexcept {
        return Control && !Control->IsEmpty();
    };

    template <typename U>
    friend class WeakPtr;

private:
    ControlBlock<T>* Control = nullptr;
};

template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (Control) {
        Control->DelShared();
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
    using element_type = T;    // Your code goes here...

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
    ControlBlock<T>* Control;
};

template<typename T>
T &SharedPtr<T>::operator*() const noexcept {
    return *(Control->GetPtr());
}

template<typename T>
T *SharedPtr<T>::operator->() const noexcept {
    return Control->GetPtr();
}

template<typename T>
int64_t SharedPtr<T>::UseCount() const noexcept {
    return Control->GetStrongCount();
}

template<typename T>
void SharedPtr<T>::Reset() noexcept {
    Control = nullptr;
}

template<typename T>
template<typename Y, typename Deleter>
SharedPtr<T>::SharedPtr(Y *p, Deleter deleter) noexcept : Control(new ControlBlock<T, Deleter>(p, deleter)) {
    Control->AddStrongPtr();
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr &other) noexcept {
    Control = other.Control;
    Control->AddStrongPtr();
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &r) noexcept {
    if (&r == this) {
        return *this;
    }

    if (Control) {
        Control->DelShared();
    }

    Control = r.Control;
    Control->AddStrongPtr();
    return *this;
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&r) noexcept {
    if (&r == this) {
        return *this;
    }

    if (Control) {
        Control->DelShared();
    }

    Control = new ControlBlock<T>();
    Control->SetPtr(r.Control->GetPtr());
    Control->SetValues(r.Control->GetStrongCount(), r.Control->GetWeakCount());
    r.Control->SetValues(0, 0);
    r.Control->SetPtr(nullptr);
    return *this;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr &&other) noexcept {
    Control = new ControlBlock<T>();
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());
    Control->SetPtr(other.Control->GetPtr());
    other.Control->SetPtr(nullptr);
    other.Control->SetValues(0, 0);
}

template<typename T>
void SharedPtr<T>::Swap(SharedPtr &other) noexcept {
    swap(Control, other.Control);
}

template<typename T>
T *SharedPtr<T>::Get() const noexcept {
    return Control->GetPtr();
}

template<typename T>
template<typename Y>
void SharedPtr<T>::Reset(Y *p) noexcept {
    SharedPtr<T>(p).Swap(*this);
}

template<typename T>
template<typename Y, typename Deleter>
void SharedPtr<T>::Reset(Y *p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).Swap(*this);
}

template<typename T>
SharedPtr<T>::SharedPtr(const WeakPtr<T> &other) noexcept {
    Control = other.Control;

    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());
    Control->AddStrongPtr();
}

template<typename T>
template<typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y> &other) {
    Control = new ControlBlock<T>();
    Control->SetPtr(other.Control->GetPtr());
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());

    Control->AddWeakPtr();
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr &other) noexcept {
    Control = new ControlBlock<T>();
    Control->SetPtr(other.Control->GetPtr());
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());

    Control->AddWeakPtr();
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr &&other) noexcept {
    Control = new ControlBlock<T>();
    Control->SetPtr(other.Control->GetPtr());
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());

    other.Control.SetPtr(nullptr);
    other.Control.SetValues(0, 0);
}

template<typename T>
template<typename Y>
WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr<Y> &other) {
    Control = other.Control;

    Control->AddWeakPtr();
    return *this;
}

template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr &other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (Control) {
        Control->DelWeak();
    }

    Control->SetPtr(other.Control->GetPtr());
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());

    Control->AddWeakPtr();
    return *this;
}

template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr &&other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (Control) {
        Control->DelWeak();
    }

    Control = new ControlBlock<T>();
    Control->SetPtr(other.Control->GetPtr());
    Control->SetValues(other.Control->GetStrongCount(), other.Control->GetWeakCount());

    other.Control.SetPtr(nullptr);
    other.Control.SetValues(0, 0);
}

template<typename T>
WeakPtr<T>::~WeakPtr() {
    Control->DelWeak();
}

template<typename T>
void WeakPtr<T>::Reset() noexcept {
    if (Control) {
        Control->SetPtr(nullptr);
        Control->SetValues(0, 0);
    }
}

template<typename T>
void WeakPtr<T>::Swap(WeakPtr<T> &other) noexcept {
    auto tmp = other.Control;
    other.Control = Control;
    Control = tmp;
}

template<typename T>
bool WeakPtr<T>::Expired() const noexcept {
    if (Control) {
        return Control->GetStrongCount() == 0;
    }
    return true;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::Lock() const noexcept {
    if (Expired()) {
        return SharedPtr<T>();
    } else {
        return SharedPtr<T>(*this);
    }
}



// WeakPtr
// Your code goes here...
// WeakPtr