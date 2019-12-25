#pragma once

#include "stdlib/types.h"
#include "stdlib/deque.h"

typedef struct snm_head_t{
    u32                 ind;
    deque               *childs;
    deque               *dfs;
    deque               *childs_all;
    bool                is_root;
    struct snm_head_t   *next;
    int                 size;
} snm_head;



typedef struct snm_t{
    u32         *frames;
    u32         nframes;
    snm_head    *heads;
    u32         *parents; 
    u32         *ranks;
} snm;


snm *init_snm(u32 *frames, u32 nframes);

void make_set(snm *s, u32 ind, snm_head *next);

void  union_snm(snm *s, snm_head *h1, snm_head *h2);

snm_head *find(snm *s, u32 ind);

void relink(snm *s, u32 ind);

void delete(snm *s, u32 ind);

void reduce_tree_delete(snm *s, u32 ind);

void find_leaf(snm *s, u32 ind);

void delete_leaf(snm *s, u32 ind);
