#ifndef MEM_MEM_H
#define MEM_MEM_H

#include "mem/vals.h"
#include "mem/cash.h"
#include "rb_tree.h"

#define CASH_SIZE 1000

#define MEM_BEG 0x200000

#define MEM_END 0x300000

void *kmallock (size_t size);

void *kmallock_a(size_t size);

void kfree (void *addr);

void init_mem();



#endif
