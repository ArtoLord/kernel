#include "mem/cash.h"
#include "writer.h"

void init_cash(size_t block_size, int entrys, size_t begin,cash_holder *holder){
    *holder = (cash_holder){
        (cash_e *)begin,
        block_size,
        entrys
    };
    if (holder->pointer==null){
        kpanic("CASH POINTER IS NULL, PANIC");
        return;
    }
    cash_e *curr = (cash_e*)(begin+block_size);
    for (int ent = 0;ent<entrys-2;ent++){
        curr->next = curr+block_size;
        curr->prev = curr-block_size;
        curr->in_use = false;
        curr = curr->next;
    }
    curr->in_use = false;
    holder->pointer->in_use = false;
    holder->pointer->next->in_use = false;
    curr->next = (cash_e*)begin;
    curr->prev = curr-block_size;
    holder->pointer->prev = curr;
    holder->pointer->next = (cash_e *)(begin+block_size);
    holder->pointer=curr;
}

size_t get_cash_size(cash_holder *holder){
    return holder->block_size*holder->cash_size;
}

cash_e  *cash_allock(cash_holder *holder){

        holder->pointer->in_use = true;
        cash_e *curr = holder->pointer;
        curr->prev->next = curr->next; 
        holder->pointer = curr->next;
        return curr;
}

void cash_free(cash_holder *holder,cash_e *curr){
    if (curr==null){
        kpanic("pointer is null, panic");
        return;
    }
    curr->next = holder->pointer;
    curr->prev = holder->pointer->prev;
    curr->prev->next = curr;
    curr->next->prev = curr;
    curr->in_use = false;
    holder->pointer = curr;
}


