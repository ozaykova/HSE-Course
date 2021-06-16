#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using pointer = T*;
    using value_type = T;
    // Your code goes here

    CustomAllocator() = default;

    CustomAllocator(const CustomAllocator& other) noexcept
        : arena_{other.arena_}, arena_offset_{other.arena_offset_}, num_(other.num_) {
        ++(*num_);
    }

    ~CustomAllocator() {
        --(*num_);
        if ((*num_) == 0) {
            ::operator delete(arena_);
            delete arena_offset_;
            delete num_;
        }
    }

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept
        : arena_{other.GetArena()},
          arena_offset_{other.GetArenaOffset()},
          num_(other.GetNumAllocators()) {
        ++(*num_);
    }

    void* GetArena() const {
        return arena_;
    }

    size_t* GetArenaOffset() const {
        return arena_offset_;
    }

    size_t* GetNumAllocators() const {
        return num_;
    }

    T* allocate(size_t n) {  // NOLINT
        pointer next_address = static_cast<pointer>(arena_) + *arena_offset_;
        *arena_offset_ += n;
        return next_address;
    }
    void deallocate(T* p, size_t n){
        // NOLINT
    };
    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        new (p) value_type{std::forward<Args>(args)...};
    };
    void destroy(pointer p) {  // NOLINT
        p->~value_type();
    };

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;
    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const std::size_t kDefaultSize{20000};
    void* arena_{::operator new(kDefaultSize * sizeof(value_type))};
    std::size_t* arena_offset_{new std::size_t(0)};
    std::size_t* num_{new std::size_t{1}};
};

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.arena_ == rhs.arena_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}