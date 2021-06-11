#pragma once

#include <atomic>

class SharedCount {
public:
    explicit SharedCount(std::size_t count = 0) noexcept : StrongCount(count) {
    }

    void AddStrongPtr() {
        StrongCount += 1;
    }

    size_t GetStrongCount() {
        return StrongCount;
    }

protected:
    std::atomic<std::size_t> StrongCount = 0;
};

class SharedWeakCount : public SharedCount {
public:
    void AddWeakPtr() {
        WeakCount += 1;
    }

    size_t GetWeakCount() {
        return WeakCount;
    }

protected:
    std::atomic<size_t> WeakCount = 0;
};

template <typename T, typename Deleter = std::default_delete<T>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock() {
        ptr_ = new T();
    }
    ControlBlock(T* ptr, Deleter deleter) : ptr_(ptr), del_(deleter) {
    }

    explicit ControlBlock(T* ptr) : ptr_(ptr), del_(std::default_delete<T>()) {
    }

    void DelShared() {
        StrongCount -= 1;
        if (NeedDel()) {
            del_(ptr_);
        }
    }

    void DelWeak() {
        WeakCount -= 1;
        if (NeedDel()) {
            del_(ptr_);
        }
    }

    bool IsEmpty() {
        return ptr_ == nullptr;
    }

    T* GetPtr() {
        return ptr_;
    }

    void SetPtr(T ptr) {
        if (&ptr) {
            ptr_ = &ptr;
        }
    }

    void SetPtr(T* ptr) {
        ptr_ = ptr;
    }

    void SetValues(size_t strong, size_t weak) {
        WeakCount = weak;
        StrongCount = strong;
    }
private:
    T* ptr_ = nullptr;
    Deleter del_;
    bool NeedDel() {
        return GetStrongCount() + GetWeakCount() == 0 && ptr_ != nullptr;
    }
};