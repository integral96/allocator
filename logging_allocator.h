#pragma once
#include <iostream>

template<typename T>
class logging_allocator 
{
    public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = logging_allocator<U>;
    };
////////////////////////////////////////////////////
    T *allocate(size_t n) {
        auto p = malloc(n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
    }
    void deallocate(T *p, size_t n) {
        free(p);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }
};
template<typename T1, typename T2>
bool operator==(const logging_allocator<T1>&, const logging_allocator<T2>&) noexcept
{
    return true;
}
template<typename T1, typename T2>
bool operator!=(const logging_allocator<T1>&, const logging_allocator<T2>&) noexcept
{
    return false;
}
