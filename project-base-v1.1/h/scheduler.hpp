//
// Created by os on 7/16/23.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "list.hpp"
#include "../h/sleepingList.hpp"

class TCB;
class SleepingList;

class Scheduler {
private:
    static List<TCB> readyCoroutineQueue;
    static SleepingList sleepingList;

public:
    static TCB* get();
    static void put(TCB* tcb);

    static int putThreadToSleep(TCB* thread, int time);
    static void updateSleepingList();
};

#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP