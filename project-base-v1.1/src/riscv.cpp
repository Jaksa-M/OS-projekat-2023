//
// Created by os on 7/16/23.
//
//Kod je preuzet sa vezbi za sinrhonu promenu konteksta
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/_sem.hpp"
#include "../h/console.hpp"
#include "../h/print.hpp"
#include "../lib/mem.h"
void Riscv::popSppSpie() {
    // dozvoljava prekide
    ms_sstatus(SSTATUS_SPIE);

    if(TCB::running->getSystemReg() == true){
        __asm__ volatile ("csrs sstatus, %[Vrednost]" :: [Vrednost] "r" (0x100)); // Sistemske privilegije
        //__asm__ volatile("csrs sstatus, %0" :: "r" (0x100)); // Sistemske privilegije
    }
    else{
        __asm__ volatile ("csrc sstatus, %[Vrednost]" :: [Vrednost] "r" (0x100)); // Korisnicke privilegije
        //__asm__ volatile("csrc sstatus, %0" :: "r" (0x100)); // Korisnicke privilegije
    }

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap(){
    //Cuvamo ova 2 registra da bi ih kasnije restaurirali
    volatile uint64 sepc = Riscv::r_sepc(); //sacuvana vrednost registra pc u korisnickom rezimu.
    volatile uint64 sstatus = Riscv::r_sstatus();

    volatile uint64 scause = Riscv::r_scause();

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {

        sepc += 4; //ecall ne povecava PC automatski, pa moramo rucno

        //Unapred cuvamo argumente funkcijskih poziva (cuvamo ih sve i bice visak u vecini slucajeva)
        //Na ovaj nacin ih cuvamo na steku, jer da smo cuvali u registrima, onda bi moglo da dodje do slucaja gde ce
        //npr csrr instrukcija da promeni neki od registara i onda bi u registru bila neka druga vrednost.
        volatile uint64 brojFunkcije;
        volatile uint64 argument1;
        volatile uint64 argument2;
        volatile uint64 argument3;
        volatile uint64 argument4;

        __asm__ volatile ("ld %[BrojFunkcije], 10 * 8(fp)": [BrojFunkcije] "=r" (brojFunkcije));
        __asm__ volatile ("ld %[Argument1], 11 * 8(fp)": [Argument1] "=r" (argument1));
        __asm__ volatile ("ld %[Argument2], 12 * 8(fp)": [Argument2] "=r" (argument2));
        __asm__ volatile ("ld %[Argument3], 13 * 8(fp)": [Argument3] "=r" (argument3));
        __asm__ volatile ("ld %[Argument4], 14 * 8(fp)": [Argument4] "=r" (argument4));

        volatile uint64 returnVal = 0;

        switch (brojFunkcije) {
            case 0x01: //void* mem_alloc (size_t size)
                returnVal = (uint64)MemoryAllocator::mem_alloc((size_t)argument1);
                //returnVal = (uint64)__mem_alloc((size_t)argument1);
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x02: //int mem_free (void*)
                returnVal = (uint64)MemoryAllocator::mem_free((void*)argument1);
                //returnVal = (uint64)__mem_free((void*)argument1);
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x11: //int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space)
                //*((uint64*)argument1) = (uint64)(TCB::createThread((TCB::Body)argument2, (TCB::Argument)argument3));
                *((uint64*)argument1) = (uint64)(TCB::createThread((TCB::Body)argument2, (TCB::Argument)argument3, (void*)argument4));
                returnVal = (argument1 == 0) ? -1 : 0;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x12: //int thread_exit ()
                returnVal = (TCB::exit() == 0) ? 0 : -1;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x13: //void thread_dispatch ()
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            case 0x14: //void thread_join (thread_t handle)
                TCB::join((TCB*) argument1);
                break;
            case 0x21: //int sem_open (sem_t* handle, unsigned init)
                *(sem_t*)(argument1) = new sem((int) argument2);
                returnVal = (argument1 == 0) ? -1 : 0;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x22: //int sem_close (sem_t handle)
                returnVal = (((sem_t)argument1)->close() == 0) ? 0 : -1;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x23: //int sem_wait (sem_t id)
                returnVal = (((sem_t)argument1)->wait() == 0) ? 0 : -1;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x24: //int sem_signal (sem_t id)
                returnVal = (((sem_t)argument1)->signal() == 0) ? 0 : -1;
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x31: //int time_sleep (time_t)
                TCB::timeSliceCounter = 0;
                returnVal = Scheduler::putThreadToSleep(TCB::running, (int)argument1);
                TCB::dispatch();
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x41: //char getc ()
                returnVal = (uint64)MyConsole::getInstance()->getc();
                __asm__ volatile ("sd %[Vrednost], 10 * 8(fp)":: [Vrednost] "r" (returnVal)); // na steku na mesto registra a0 stavljamo returnVal
                break;
            case 0x42: //void putc (char)
                MyConsole::getInstance()->putc(argument1);
                break;
            default: //Ovde nikad ne bi trebalo da udje, ako udje, onda je greska
                MyConsole::getInstance()->putc('h');
                break;
        }
    }
    else { // unexpected trap cause
        printIntegerMine(scause);
    }
    //Restauriramo sacuvane registre
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void Riscv::handleTimerTrap() { // Prekid od tajmera
    volatile uint64 sepc = Riscv::r_sepc(); //sacuvana vrednost registra pc u korisnickom rezimu.
    volatile uint64 sstatus = Riscv::r_sstatus();

    Riscv::mc_sip(Riscv::SIP_SSIP); // softverski prekid obradjen
    Scheduler::updateSleepingList();
    TCB::timeSliceCounter++;
    if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
    }

    //Restauriramo sacuvane registre
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void Riscv::handleConsoleTrap() { // Prekid od konzole
    volatile uint64 sepc = Riscv::r_sepc(); //sacuvana vrednost registra pc u korisnickom rezimu.
    volatile uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sip(Riscv::SIP_SSIP);

    int id = plic_claim(); // Povratna vrednost ove funkcije je broj prekida (za consolu 10).
    //__asm__ volatile("csrc sip, %0" ::"r"(1UL<<9));
    __asm__ volatile ("csrc sip, %[Vrednost]" :: [Vrednost] "r" (1UL<<9));
    sem_signal(MyConsole::getInstance()->semGet);
    sem_signal(MyConsole::getInstance()->semPut);
    plic_complete(id); // Obavestavamo kontroler prekida da je prekid obradjen.
    //console_handler();

    //Restauriramo sacuvane registre
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}