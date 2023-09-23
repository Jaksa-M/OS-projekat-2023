//
// Created by os on 4/16/23.
//

#include "../h/MemoryAllocator.hpp"

typedef MemoryAllocator::FreeMemory FreeMemory;
FreeMemory* MemoryAllocator::head = nullptr;

void* MemoryAllocator::mem_alloc(size_t size){ //First fit algoritam
    if(size == 0) return nullptr; // Nista ne treba alocirati

    // Parametar size je prosledjen u blokovima a ne u bajtovima.
    if(head == nullptr) {
        head = (FreeMemory*) HEAP_START_ADDR;
        //head->size = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR; // Na prvoj alokaciji, velicina slobogdnog fragmenta je ceo heap.
        head->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR)/MEM_BLOCK_SIZE; // Na prvoj alokaciji, velicina slobogdnog fragmenta je ceo heap.
        head->next = nullptr;
    }
    FreeMemory* prev = nullptr;
    FreeMemory* free_segment = head;
    void* segment_for_process;

    // Pronalazimo slobodan segment koji je dovoljne velicine.
    while(free_segment != nullptr && size > free_segment->size){
        prev = free_segment;
        free_segment = free_segment->next;
    }
    if(free_segment == nullptr) return nullptr; // Ne postoji dovoljno veliki free segment za alokaciju.

    segment_for_process = (void*) free_segment;
    size_t changed_size = free_segment->size - size;
    *(size_t*)segment_for_process = size; // svaki alocirani segment na pocetku ima jedno polje tipa size_t koje sadrzi velicinu tog segmenta
    FreeMemory* next = free_segment->next;
    if(changed_size > 0){ // Moramo da suzimo free segment
        free_segment = (FreeMemory*)((char*)free_segment + size*MEM_BLOCK_SIZE); // pomerili smo adresu free_segmenta posto je suzen.
        free_segment->size = changed_size;
        free_segment->next = next;
        if(prev != nullptr){
            prev->next = free_segment;
        }
        else{
            head = free_segment;
        }
    }
    else{ // ukoliko smo alocirali prostor koji je iste velicine kao slobodan segment
        if(prev != nullptr){
            prev->next = next;
        }
        else{
            head = next;
        }
    }

    return (void*)((char*)segment_for_process + sizeof(size_t));
}

int MemoryAllocator::mem_free(void* ptr){
    if(ptr == nullptr) return -1;
    if (!(ptr >= HEAP_START_ADDR && ptr < HEAP_END_ADDR)) return -1;

    FreeMemory* current = head;
    //FreeMemory* segment_to_be_freed = (FreeMemory*) ((char*)ptr - (size_t)sizeof(size_t));
    FreeMemory* segment_to_be_freed = (FreeMemory*) ((size_t*)ptr - 1);
    FreeMemory* prev = nullptr;
    bool connected = false; // promenljiva koja sluzi za proveru da li je spojeno sa narednim ili prethodnim segmentom, ili uopste nije spojeno.

    // Iteriramo kroz free segmente sve dok je adresa free segmenta manja od adrese zauzetog
    while(current != nullptr && current < segment_to_be_freed){
        prev = current;
        current = current->next;
    }
    // U promenljivoj prev se nalazi adresa slobodnog segmenta koji je prvi slobodan segment pre zauzetog koji oslobadjamo.

    // Proveravamo da li se segment_to_be_freed nalazi unutar slobodnog fragmenta (oslobadjanje vec oslobodjenog prostora).
    if(prev != nullptr && ((char*)segment_to_be_freed > (char*)prev) && ((char*)segment_to_be_freed < (char*)prev + prev->size*MEM_BLOCK_SIZE)) return -1;

    // Slucaj kada mozemo da spojimo sledeci slobodni segment sa novooslobodjenim u jedan slobodni segment.
    if(current != nullptr && (char*)segment_to_be_freed + segment_to_be_freed->size*MEM_BLOCK_SIZE == (char*)current){
        connected = true;
        segment_to_be_freed->size += current->size;
        segment_to_be_freed->next = current->next;
        if(prev != nullptr){
            prev->next = segment_to_be_freed;
        }
        else{
            head = segment_to_be_freed;
        }
    }
    // Slucaj kada mozemo da spojimo prethodni slobodni segment sa novooslobodjenim u jedan slobodni segment.
    if(prev != nullptr && (char*)prev + prev->size*MEM_BLOCK_SIZE == (char*)segment_to_be_freed){
        connected = true;
        prev->size += segment_to_be_freed->size;
        prev->next = segment_to_be_freed->next;
    }
    // Slucaj kada ne mozemo da spojimo ni sa prethodnim ni sa sledecim free segmentom.
    if(connected == false){
        if(prev != nullptr) {
            prev->next = segment_to_be_freed;
            segment_to_be_freed->next = current;
        }
        else{
            segment_to_be_freed->next = head;
            head = segment_to_be_freed;
        }
    }
    return 0;
}