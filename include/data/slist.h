#pragma once

#include <stdlib/types.h>
#include <mem/mem.h>
#include <stdlib/deque.h>

#define llist_e struct llist_e_t

struct llist_e_t{
    llist_e     *next;
    llist_e     *prev;
}attribute(packed);

#define llist struct llist_t

llist{
    int size;
    llist_e     *front;
    llist_e     *back;
    llist       *free;
}attribute(packed);

llist * init_llist(llist *);

#define llist_add(list, data, after, befor) \
    if ( !=null){\
        llist *add_entry = front(deque)\
        pop_front(deque)\
    }else{\
        llist_e *add_entry = kmallock(sizeof(llist_e)+sizeof(data));\
    }\
    \
    typeof(data) *add_data = (typeof(data))(add_entry+sizeof(llist_e));\
    *add_data = data;\
    *add_entry = (llist_e){\
        befor,\
        after\
    };\
    befor->prev = add_entry;\
    after->next = add_entry;\
    

void llist_delete(llist *, llist_e *);

void llist_delete_all(llist *);

void llist_free(llist*);

void llist_push_list(llist *, llist_e * after, llist_e * before);



