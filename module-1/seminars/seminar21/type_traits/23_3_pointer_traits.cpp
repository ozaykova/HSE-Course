#include <cstdint>
#include <type_traits>

template <typename T>
struct PointerTraits;

template <typename T>
struct PointerTraits<T*> {
    using pointer = T*;
    using element_type = T;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    using rebind = U*;

    // Attempt 3: we will try to handle T before usage inside parameter list
    // put T to enable_if_t -> use result type (TR) in the signature
    // Result: CE again
    // Reson: we have T& at enable_if_t (T=void -> void& -> CE)
    template <bool B = std::is_void_v<T>, typename TR = std::enable_if_t<!B, T&>>
    static auto PointerTo(TR r) {
        return &r;
    }
};

int main() {
    int32_t x = 1;

    int32_t* ptr_x = PointerTraits<int32_t*>::PointerTo(x);
    void* ptr_void = PointerTraits<void*>::PointerTo(x);
    return 0;
}