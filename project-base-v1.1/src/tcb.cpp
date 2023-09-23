#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB* TCB::createThread(Body body, Argument arg, void* stack_space) {
    return new TCB(body, TIME_SLICE, arg, stack_space);
}
//TCB* TCB::createThread(Body body, Argument arg, void* stack_space) {
//    return new TCB(body, TIME_SLICE, arg, stack_space);
//}

void TCB::setThreadState(TCB::threadState state) {
    this->state = state;
}

TCB::threadState TCB::getThreadState() {
    return state;
}

void TCB::join(TCB* t) {
    if(t->isFinished() == true) {
        return;
    }
    t->joinQueue.addLast(running);
    running->setThreadState(SLEEPING);
    thread_dispatch();
}

int TCB::exit() {
    // Praznimo joinQueue tekuce niti.
    while(running->joinQueue.peekFirst() != nullptr){
        running->joinQueue.peekFirst()->setThreadState(RUNNING);
        Scheduler::put(running->joinQueue.removeFirst());
    }
    if(running->state == RUNNING){
        running->state = FINISHED;
        thread_dispatch();
        return 0;
    }
    return -1;
}

void TCB::yield() {
    thread_dispatch();
}

void TCB::dispatch() {
    TCB *old = running;
    if (old->isRunning() == true) {
        Scheduler::put(old); //stari running stavimo u scheduler
    }
    running = Scheduler::get(); //uzmemo jednu nit iz schedulera i postavimo je running
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg); // Pokrenemo telo niti da se izvrsava.
    // Kad smo na ovom delu, nit je zavrsila svoj posao(telo) i sada praznimo joinQueue te niti.
    while(running->joinQueue.peekFirst() != nullptr){
        running->joinQueue.peekFirst()->setThreadState(RUNNING);
        Scheduler::put(running->joinQueue.removeFirst());
    }
    running->setFinished(true);
    thread_dispatch();
}