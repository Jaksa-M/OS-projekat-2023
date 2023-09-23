//
// Created by os on 4/16/23.
//
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/console.hpp"


void idle(void* arg){ // nit koja ce uvek biti u scheduleru i ne raditi nista
    while(1){
        thread_dispatch();
    }
}
extern "C" void IntVector();

void userMain();

void testWrapper(void* ptr){
    userMain();
}

int main()
{
    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::w_stvec((uint64) &IntVector|1UL);

    TCB *threads[8];

    thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];
    TCB::running->setSystemReg(true);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&threads[1], &MyConsole::getInstance()->getcFunctThread, nullptr);
    printStringMine("ThreadGetConsole created\n");
    thread_create(&threads[2], &MyConsole::getInstance()->putcFunctThread, nullptr);
    printStringMine("ThreadPutConsole created\n");
    thread_create(&threads[7], &(idle), nullptr);
    printStringMine("IDLE Thread created\n");

    thread_create(&threads[3], &testWrapper, nullptr);
    thread_join(threads[3]);


    for (auto &thread: threads) {
        delete thread;
    }

    return 0;
}