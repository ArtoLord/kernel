#include "stdlib/deque.h"


deque *new_deque(){
    deque * curr = kmallock(sizeof(deque));
    deque * free = kmallock(sizeof(deque));
    *free = (deque){null, null, null};
    *curr = (deque){\
        null,\
        null,\
        free
    };
    return curr;
}

void pop_back(deque *d){
    if (d->back==null) return;
    deque_e *c = d->back;
    d->back = d->back->prev;
    if (d->back==null) {
        d->front = null;
        return;
    }
    d->back->next=null;
    kfree(c);
}

void pop_front(deque *d){
    if (d->front==null) return;
    deque_e * c = d->front;
    d->front = d->front->next;
    if (d->front==null) {
        d->back = null;
        return;
    }
    d->front->prev=null;
    kfree(c);
}

void *front(deque *d){
    return GET_DEQUE_DATA(d->front);
}

void *back(deque *d){
    return GET_DEQUE_DATA(d->back);
}

void clear(deque * d){
    if (d->free->back == null){
        d->free->front = d->front;
        d->free->back = d->back;
    }
    d->front = null;
    d->back = null;
}

void destruct_deque(deque *d){
    while (d->front != null){
        pop_front(d);
    }
    while (d->free->front!=null){
        pop_front(d->free);
    }
}
