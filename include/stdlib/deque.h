#pragma once

#define deque_e struct deque_e_t

#include "mem/mem.h"
#include "writer.h"
#include "types.h"

struct deque_e_t{
    deque_e       *next;
    deque_e       *prev;
};

typedef struct deque_t{
    deque_e    *front;
    deque_e    *back;
    struct deque_t         *free;
} deque;

#define GET_DEQUE_DATA(ptr) (void *)(ptr + sizeof(deque_e))

#define push(d, after, before, data) {\
deque_e *ent = null;\
if(d->free->front==null){\
ent = kmallock(sizeof(deque_e)+sizeof(data));\
}\
else {ent = d->free->front; pop_front(d->free);}\
*ent = (deque_e){\
              after,\
              before,\
};\
if (after==null) {\
    d->front = ent;\
}\
else{\
    after->next = ent;\
    ent->prev = after;\
}\
if (before==null) {\
    d->back = ent;\
}\
else{\
    before->prev = ent;\
    ent->next = before;}\
*(typeof(data) *)(ent+sizeof(deque_e)) = data;}

#define push_front(d, data) \
push(d,null,d->front,data)

#define push_back(d,data) push(d,d->back, null, data)

void pop_back(deque *d);

void pop_front(deque *d);

void *front(deque *d);

void *back(deque *d);

void clear(deque *d);

deque *new_deque();
