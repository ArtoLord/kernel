#ifndef MEM_CASH_H
#define MEM_CASH_H

#include "mem/vals.h"
#include "stdlib/types.h"

void init_cash(size_t block_size, int entrys, size_t begin, cash_holder *holder);

size_t get_cash_size(cash_holder *holder);

cash_e *cash_allock(cash_holder *holder);
void cash_free(cash_holder *holder, cash_e *entry);


#endif
