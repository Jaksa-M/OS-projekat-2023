//S
// Created by os on 7/16/23.
//

#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

List<TCB> Scheduler::readyCoroutineQueue;
SleepingList Scheduler::sleepingList;

TCB* Scheduler::get(){
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(TCB* tcb){
    readyCoroutineQueue.addLast(tcb);
}

int Scheduler::putThreadToSleep(TCB* thread, int time) {
    if(thread == nullptr || time < 0) return -1;
    if(time == 0) return 0;
    sleepingList.put(thread, time);
    return 0;
}

void Scheduler::updateSleepingList() {
    sleepingList.update();
}