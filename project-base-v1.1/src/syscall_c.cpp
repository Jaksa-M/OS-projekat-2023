//
// Created by os on 7/17/23.
//

#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" ((size + sizeof(uint64) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));
    //__asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (size));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x01));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (void*)retValue;
}

int mem_free (void* ptr){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (ptr));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x02));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    void* stack = (char*)(new char[DEFAULT_STACK_SIZE]);
    __asm__ volatile ("mv a4, %[Vrednost]": : [Vrednost] "r" (stack));
    __asm__ volatile ("mv a3, %[Vrednost]": : [Vrednost] "r" (arg));
    __asm__ volatile ("mv a2, %[Vrednost]": : [Vrednost] "r" (start_routine));
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (handle));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x11));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int thread_exit(){
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x12));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

void thread_dispatch(){
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x13));
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (handle));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x14));

    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init){
    __asm__ volatile ("mv a2, %[Vrednost]": : [Vrednost] "r" (init));
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (handle));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x21));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int sem_close (sem_t handle){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (handle));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x22));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int sem_wait (sem_t id){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (id));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x23));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int sem_signal(sem_t id){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (id));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x24));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

int time_sleep (time_t time){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (time));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x31));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (int) retValue;
}

char getc(){
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x41));

    __asm__ volatile("ecall");

    volatile uint64 retValue;
    __asm__ volatile ("mv %[PovratnaVrednost], a0": [PovratnaVrednost] "=r" (retValue));
    return (char) retValue;
}

void putc(char c){
    __asm__ volatile ("mv a1, %[Vrednost]": : [Vrednost] "r" (c));
    __asm__ volatile ("mv a0, %[Entry]": : [Entry] "r" (0x42));

    __asm__ volatile("ecall");
}