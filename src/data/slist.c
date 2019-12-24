#include <data/slist.h>

llist * init_llist(llist * list){
    llist *free = (llist *)kmallock(sizeof(llist));
    *free = (llist){
        0,
        null,
        null,
        null
    };
    *list = (llist){
        0,
        null,
        null,
        free
    };
    return list;
}



void llist_delete(llist * list, llist_e *entry, bool free){
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    list->size--;
    if (free){
        kfree(entry);
    }
    else{
        push_back(list->free, entry);
    }
}

void llist_delete_all(llist * list){
    list->free->back->next = list->front;
    list->free->back = list->back;
};
