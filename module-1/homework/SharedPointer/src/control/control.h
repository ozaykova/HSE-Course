#pragma once

#include <atomic>

using namespace std;

class SharedCount {
public:
    explicit SharedCount(std::size_t count = 0) noexcept: StrongCount(count) {}
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

template <typename T, typename Deleter=std::default_delete<T>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock() {Ptr = new T();}
    ControlBlock(T* ptr, Deleter deleter): Ptr(ptr), Del(deleter) {}
    ControlBlock(T* ptr) : Ptr(ptr), Del(std::default_delete<T>()) {}


    void DelShared() {
        StrongCount -= 1;
        if (NeedDel()) {
            Del(Ptr);
        }
    }

    void DelWeak() {
        WeakCount -= 1;
        if (NeedDel()) {
            Del(Ptr);
        }
    }

    bool IsEmpty() {
        return Ptr == nullptr;
    }

    T* GetPtr() {
        return Ptr;
    }

    void SetPtr(T ptr) {
        if (&ptr) {
            Ptr = &ptr;
        }
    }

    void SetPtr(T* ptr) {
        Ptr = ptr;
    }

    void SetValues(size_t strong, size_t weak) {
        WeakCount = weak;
        StrongCount = strong;
    }
private:
    T* Ptr = nullptr;
    Deleter Del;
    bool NeedDel() {
        if (GetStrongCount() + GetWeakCount() == 0 && Ptr != nullptr) {
            return true;
        }
        return false;
    }
};