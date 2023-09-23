//
// Created by os on 7/20/23.
//

#ifndef PROJECT_BASE_V1_1__SEM_HPP
#define PROJECT_BASE_V1_1__SEM_HPP

#include "../lib/hw.h"
#include "list.hpp"
#include "syscall_c.hpp"

class sem {
public:
    sem(int init = 1);
    int wait();
    int signal();
    void block();
    void unblock();
    int close();

private:
    List<TCB> blockedThreads;
    int val;
    int closed;
};

#endif //PROJECT_BASE_V1_1__SEM_HPP