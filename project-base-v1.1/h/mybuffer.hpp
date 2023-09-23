//
// Created by os on 8/1/23.
//

#ifndef PROJECT_BASE_V1_1_MYBUFFER_HPP
#define PROJECT_BASE_V1_1_MYBUFFER_HPP

#include "_sem.hpp"

const int N = 1000; //velicina buffera

// Kod preuzet iz knjige, strana 278
class MyBuffer {
private:
    sem_t mutex; // ovo je mutex semafor koji sluzi za medjusobno iskljucenje.
    sem_t spaceAvailable, itemAvailable;
    //spaceAvailable semafor koristi put nit da proveri da li moze da stavi karakter u bafer, tj da li je bafer pun.
    //itemAvailable semafor koristi get nit da proveri da li ima itema u baferu za uzimanje.

    char buffer[N];
    int head, tail;
public:
    MyBuffer();
    void put(char c);
    char get();
};

#endif //PROJECT_BASE_V1_1_MYBUFFER_HPP