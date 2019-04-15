#ifndef LOGGING_ALLOCATOR_H
#define LOGGING_ALLOCATOR_H
#include <iostream>
using namespace std;

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
        cout <<__PRETTY_FUNCTION__ << "[n = " << n << "]" << endl;
        auto p = malloc(n * sizeof(T));
        if (!p)
            throw bad_alloc();
        return reinterpret_cast<T *>(p);
    }
    void deallocate(T *p, size_t n) {
        cout << __PRETTY_FUNCTION__ <<"[n = " << n << "]" << endl;
        free(p);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        cout << __PRETTY_FUNCTION__ << endl;
        new(p) U(forward<Args>(args)...);
    };

    void destroy(T *p) {
        cout << __PRETTY_FUNCTION__ << endl;
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



#endif // LOGGING_ALLOCATOR_H