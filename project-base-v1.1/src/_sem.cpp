//
// Created by os on 7/20/23.
//

#include "../h/_sem.hpp"
#include "../h/tcb.hpp"

sem::sem(int init) {
    this->val = (int)init;
    this->closed = 0;
}

int sem::wait() {
    if(closed == 1) return -1;
    if(--val < 0) block();
    return 0;
}

int sem::signal() {
    if(closed == 1) return -1;
    if(++val <= 0) unblock();
    return 0;
}

void sem::block() {
    blockedThreads.addLast(TCB::running);
    TCB::running->setThreadState(TCB::BLOCKED);
    thread_dispatch();
}

void sem::unblock() {
    TCB* thread = blockedThreads.removeFirst();
    if(thread == nullptr) {
        return;
    }
    thread->setThreadState(TCB::RUNNING);
    Scheduler::put(thread);
}

int sem::close() {
    if(closed == 1) return -1;
    // Kada zatvorimo semafor, sve niti koje su blokirane nad njim treba da se stave u red spremnih (Scheduler).
    while(blockedThreads.peekFirst() != nullptr){
        blockedThreads.peekFirst()->setThreadState(TCB::RUNNING);
        Scheduler::put(blockedThreads.removeFirst());
    }
    closed = 1;
    return 0;
}