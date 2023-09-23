//
// Created by os on 4/16/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator{
private:
    MemoryAllocator() = default;

public:
    struct FreeMemory{
        size_t size; //Velicina u blokovima
        FreeMemory* next = nullptr;
    };

    MemoryAllocator(const MemoryAllocator& obj) = delete;
    MemoryAllocator(MemoryAllocator&& obj) = delete;

    static FreeMemory* head;
    static void* mem_alloc(size_t);
    static int mem_free(void*);
};

#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP