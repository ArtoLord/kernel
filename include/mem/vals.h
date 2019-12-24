#ifndef MEM_H
#define MEM_H

#include "stdlib/types.h"
#include "rb_tree.h"
#define mem_block_t struct mem_block

#define  cash_e struct cash_e_t
struct mem_block{
    size_t          addr;
    size_t          size;
    bool            used;
    mem_block_t     *prev;
    mem_block_t     *next;
    void            *cash_pointer;
};


struct cash_e_t{
    bool        in_use;
    cash_e      *next;
    cash_e      *prev;
};

typedef struct {
    cash_e      *pointer;
    size_t      block_size;
    size_t      cash_size;
} cash_holder;


typedef struct {
    cash_e      cash_entry;
    mem_block_t entry;
} mem_block_cash;

typedef struct{
    cash_e      cash_entry;
    rb_e        entry;
} rb_e_cash;

#endif
