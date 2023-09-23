//
// Created by os on 8/1/23.
//

#include "../h/console.hpp"
#include "../h/tcb.hpp"

MyConsole* MyConsole::instancePtr = nullptr;

void MyConsole::putc(char c) {
    outputBuf.put(c);
}

char MyConsole::getc() {
    return inputBuf.get();
}

MyConsole *MyConsole::getInstance() {
    // Ako ne postoji instanca ove klase onda je napravimo.
    if(instancePtr == nullptr){
        instancePtr = new MyConsole();
    }
    // Ako postoji instanca ove klase, onda vracamo tu instancu (ne pravimo novi objekat klase).
    return instancePtr;
}

// Uzima sa konzole i stavlja u inputBuffer.
void MyConsole::getcFunctThread(void *p) {
    MyConsole* konzola;
    while(TCB::running->isFinished() == false){
        konzola = MyConsole::getInstance();
        sem_wait(konzola->semGet);
        while (*((char*)CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT){
            konzola->inputBuf.put(*((char*)CONSOLE_RX_DATA));
        }
    }
}

// Uzima iz outputBuffera i stavlja na konzolu.
void MyConsole::putcFunctThread(void *p) {
    MyConsole* konzola;
    while(TCB::running->isFinished() == false){
        konzola = MyConsole::getInstance();
        sem_wait(konzola->semPut);
        while (*((char*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT){
            char temp = konzola->outputBuf.get();
            if(temp == '\r'){
                temp = '\n';
            }
            *((char *) CONSOLE_TX_DATA) = temp;
//            if(temp != '\x1b') {
//                *((char *) CONSOLE_TX_DATA) = temp;
//            }
        }
    }
}