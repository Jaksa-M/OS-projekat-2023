#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    static TCB *running;

    enum threadState{RUNNING, FINISHED, BLOCKED, SLEEPING}; // Stanja u kojima nit moze da bude.

    void setThreadState(threadState state);
    threadState getThreadState();

    bool isFinished() const {
        return state == FINISHED;
    }
    bool isRunning() const {
        return state == RUNNING;
    }
    void setFinished(bool value) {
        if (value == true) state = FINISHED;
        else state = RUNNING;
    }

    bool getSystemReg(){
        return this->systemReg;
    }
    void setSystemReg(bool sysReg){
        this->systemReg = sysReg;
    }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);
    using Argument = void*;

    static TCB *createThread(Body body,  Argument arg, void* stack_space);
    //static TCB *createThread(Body body,  Argument arg);

    static void yield();
    static int exit();
    static void join(TCB* t);


private:
    TCB(Body body, uint64 timeSlice, Argument arg, void* stack_space) :
            body(body),
            //stack(body != nullptr ? new char[STACK_SIZE] : nullptr),
            stack(body != nullptr ? (char*)stack_space : nullptr), //u slucaju main funkcije, njeno telo ce biti nullptr
            context({(uint64) &threadWrapper,
                     //stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0 //u slucaju main funkcije, on vec ima svoj stek pa ne pravimo novi
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0 //u slucaju main funkcije, on vec ima svoj stek pa ne pravimo novi
                    }),
            timeSlice(timeSlice), //vreme na koje se menja kontekst
            arg(arg),
            systemReg(false)
    {
        state = RUNNING;
        if (body != nullptr) {  //ako je body = nullptr onda se radi o mainu i ne treba da ga stavimo u scheduler jer se on u tom trenutku izvrsava
            Scheduler::put(this);
        }
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    char* stack;
    Context context;
    uint64 timeSlice;
    threadState state;
    List<TCB> joinQueue;
    Argument arg;
    bool systemReg; // Promenljiva koja nam govori da li nit sme da pristupa sistemskim registrima ili ne.

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 4096;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;

    friend class Riscv;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP