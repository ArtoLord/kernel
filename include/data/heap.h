#pragma once

#include <mem/vals.h>
#include <mem/cash.h>
#include <rb_tree.h>

typedef struct heap_t {
    rb_t            *tree_used;
    rb_t            *tree_unused;
    rb_t            *tree_used_to_unused;
    cash_holder     *mem_holder;
    cash_holder     *rb_holder;
} heap;

#define TREE_U_O  0
#define TREE_UN_O  TREE_U_O + sizeof(rb_t)
#define TREE_U_T_UN_O  TREE_UN_O + sizeof(rb_t)
#define M_HOLDER_O  TREE_U_T_UN_O + sizeof(rb_t)
#define RB_HOLDER_O  M_HOLDER_O + sizeof(cash_holder)
#define CASH_O  RB_HOLDER_O + sizeof(cash_holder)

#define GET_HEAP(heap) rb_t *tree_used  = heap->tree_used;\
                       rb_t *tree_unused = heap->tree_unused;\
                       rb_t *tree_used_to_unused = heap->tree_used_to_unused;\
                       cash_holder *mem_holder = heap->mem_holder;\
                       cash_holder *rb_holder = heap->rb_holder

heap *init_heap(heap *h, void *mem_begin, void *mem_end, size_t cash_size);

void *hmallock(heap *h, size_t size);
void hfree(heap *h, void *addr);
