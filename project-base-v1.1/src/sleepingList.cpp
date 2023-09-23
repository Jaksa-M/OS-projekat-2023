//
// Created by os on 7/21/23.
//

#include "../h/sleepingList.hpp"
#include "../h/tcb.hpp"

/* SleepingLista funkcionise tako sto put stavlja na kraj liste, a update prolazi kroz celu listu
i svima smanjuje vreme za 1 i ukoliko je vreme <=0, izbacuje ga iz liste. */


void SleepingList::put(TCB* thread, int time){
    Elem* elem = new Elem(thread, time);
    if(head == nullptr){
        head = elem;
        tail = head;
    }
    else{
        tail->next = elem;
        tail = tail->next;
    }
    elem->thread->setThreadState(TCB::SLEEPING);
}

void SleepingList::update() {
    Elem* temp = head;
    Elem* prev = nullptr;
    while(temp != nullptr){
        temp->time--;
        if(temp->time <= 0){
            if(temp == head){
                if(tail == head){
                    tail = head->next;
                }
                head = head->next;
            }
            else if(temp == tail){
                tail = prev;
                tail->next = nullptr;
            }
            else{
                prev->next = temp->next;
            }
            temp->thread->setThreadState(TCB::RUNNING);
            Scheduler::put(temp->thread);
            // potrebno je obrisati temp sada
            Elem* remove = temp;
            temp = temp->next;
            delete remove;
        }
        else {
            prev = temp;
            temp = temp->next;
        }
    }
}


//void SleepingList::put(TCB* thread, int time){
//    Elem* elem = new Elem(thread, time);
//    int sumtime = 0;
//
//    if(head == nullptr){ // samo kad je lista prazna se udje ovde
//        head = elem;
//        head->next = nullptr;
//    }
//    else{
//        Elem* prev = nullptr;
//        for(Elem* temp = head; temp != nullptr; temp = temp->next){
//            sumtime += temp->time;
//            if(temp->time < sumtime){ // Nasli smo mesto u listi na koje treba da ubacimo element.
//                if(prev != nullptr) {
//                    prev->next = elem;
//                    elem->next = temp;
//                }
//                else{ // slucaj kada treba da se ubaci na mesto head-a
//                    head = elem;
//                    elem->next = temp;
//                }
//                break;
//            }
//            else{ // ako se doslo do kraja liste i nije nadjen nijedan
//                prev->next = elem;
//                elem->next = nullptr;
//            }
//            prev = temp;
//        }
//        elem->time -= sumtime;
//    }
//    elem->thread->setThreadState(TCB::SLEEPING);
//}
//void SleepingList::update() {
//    if(head == nullptr) return;
//    Elem* temp = head;
//    temp->time--;
//    if(temp->time <= 0) { //izbacujemo iz liste
//        head = head->next;
//        temp->thread->setThreadState(TCB::RUNNING);
//        Scheduler::put(temp->thread);
//        delete temp;
//    }
//}