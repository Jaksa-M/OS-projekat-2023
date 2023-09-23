//
// Created by os on 8/5/23.
//

#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    myHandle = nullptr;
    body = &wrapper;
    arg = this;
}

Thread::~Thread() {
    if(myHandle != nullptr) {
        delete myHandle;
    }
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

void Thread::join() {
    thread_join(myHandle);
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void* thread) {
    ((Thread*)thread)->run();
}

//-----------------------------------------------------------------------------------------

PeriodicThread::PeriodicThread (time_t period): Thread(&periodicWrapper, this) {
    this->period = period;
    running = true;
}

void PeriodicThread::periodicWrapper(void* periodicThread) {
    time_t p = ((PeriodicThread*)periodicThread)->period;
    while(((PeriodicThread*)periodicThread)->running == true){
        ((PeriodicThread*)periodicThread)->periodicActivation();
        time_sleep(p);
    }
}

void PeriodicThread::terminate() {
    running = false;
}

//-----------------------------------------------------------------------------------------

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

//-----------------------------------------------------------------------------------------
char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}