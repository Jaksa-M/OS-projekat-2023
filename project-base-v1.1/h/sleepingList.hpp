//
// Created by os on 7/21/23.
//

#ifndef PROJECT_BASE_V1_1_SLEEPINGLIST_HPP
#define PROJECT_BASE_V1_1_SLEEPINGLIST_HPP

class TCB;

class SleepingList{
private:
    struct Elem{
        TCB* thread;
        int time;
        Elem* next = nullptr;

        Elem(TCB* thread, int time): thread(thread), time(time){}
    };
    Elem* head;
    Elem* tail; //poslednji element u listi
public:
    SleepingList(): head(nullptr), tail(nullptr){}
    SleepingList &operator =(const SleepingList&) = delete;
    void put(TCB* thread, int time);
    void update();
};

#endif //PROJECT_BASE_V1_1_SLEEPINGLIST_HPP
