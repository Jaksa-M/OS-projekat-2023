#include "../lib/mem.h"
#include "../h/syscall_c.hpp"

using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    return mem_alloc(n);
    //return __mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
    //return __mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
    //__mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
    //__mem_free(p);
}

//
//void *operator new(size_t n)
//{
//    return __mem_alloc(n);
//    //return __mem_alloc(n);
//}
//
//void *operator new[](size_t n)
//{
//    return __mem_alloc(n);
//    //return __mem_alloc(n);
//}
//
//void operator delete(void *p) noexcept
//{
//    __mem_free(p);
//    //__mem_free(p);
//}
//
//void operator delete[](void *p) noexcept
//{
//    __mem_free(p);
//    //__mem_free(p);
//}