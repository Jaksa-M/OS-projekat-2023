//
// Created by os on 8/1/23.
//

#ifndef PROJECT_BASE_V1_1_CONSOLE_HPP
#define PROJECT_BASE_V1_1_CONSOLE_HPP

#include "mybuffer.hpp"

// Koncept singletone klase vidjen na GeeksforGeeks sajtu (singletone c++)
// Svrha singletone klase je da postoji samo jedan objekat ove klase

class MyConsole {
public:
    sem_t semGet;
    sem_t semPut;
    MyBuffer inputBuf; //Uzima sa konzole, koristi ga getc
    MyBuffer outputBuf; //Stavlja na konzolu, koristi ga putc

    //Funkcije koje ce se pozivati u sistemskom pozivu
    void putc(char c);
    char getc();

    //Funkcije koje ce pozivati niti koje su zaduzene za konzolu
    static void getcFunctThread(void* p);
    static void putcFunctThread(void* p);

    static MyConsole* getInstance();

    ~MyConsole(){
        sem_close(semGet);
        sem_close(semPut);
    }

private:
    static MyConsole* instancePtr;
    // Konstruktor u privatnoj sekciji da bi se osiguralo da se objekat ove klase moze dobiti samo koriscenjem getInstance().
    MyConsole(){
        sem_open(&semGet, 0);
        sem_open(&semPut, 0);
    }
};

#endif //PROJECT_BASE_V1_1_CONSOLE_HPP