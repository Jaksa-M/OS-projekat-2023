//
// Created by os on 8/1/23.
//

#include "../h/mybuffer.hpp"

MyBuffer::MyBuffer(): head(0), tail(0) {
    sem_open(&mutex,1);
    sem_open(&spaceAvailable,N);
    sem_open(&itemAvailable,0);
}

void MyBuffer::put(char c) {
    sem_wait(spaceAvailable);
    sem_wait(mutex);
    buffer[tail] = c;
    tail = (tail + 1) % N;
    sem_signal(mutex);
    sem_signal(itemAvailable);
}

char MyBuffer::get() {
    sem_wait(itemAvailable);
    sem_wait(mutex);
    char c = buffer[head];
    head = (head + 1) % N;
    sem_signal(mutex);
    sem_signal(spaceAvailable);
    return c;
}