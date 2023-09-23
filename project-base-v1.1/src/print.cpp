//
// Created by os on 7/16/23.
//
//Kod je preuzet sa vezbi za sinrhonu promenu konteksta
#include "../h/print.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

void printStringMine(char const* string){
    //uint64 sstatus = Riscv::r_sstatus(); //cuvanje stanja dozvoljenosti prekida
    //Riscv::mc_sstatus(Riscv::SSTATUS_SIE); //zabranjivanje prekida tokom ispisa stringa

    while(*string != '\0'){
        putc(*string);
        string++;
    }

    //Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0); //vracanje stanja dozvoljenosti prekida koje je bilo pre zabrane
}

void printIntegerMine(uint64 integer){
    //uint64 sstatus = Riscv::r_sstatus();
    //Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(integer < 0){
        neg = 1;
        x = -integer;
    }
    else{
        x = integer;
    }

    i = 0;
    do{
        buf[i++] = digits[x % 10];
    } while((x /= 10) != 0);
    if(neg){
        buf[i++] = '-';
    }

    while(--i >= 0){
        putc(buf[i]);
    }

    //Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}
