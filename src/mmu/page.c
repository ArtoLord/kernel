#include <mmu/page.h>
#include <data/heap.h>
#include <mem/mem.h>

heap *page_heap = null;

void init_mmu(){
    init_heap(page_heap,(void *) MMU_BEG, (void *)MMU_END, PAGE_NUM);
}
page *allock_page(){
    int tmp = (int)hmallock(page_heap,1);
    page *curr = kmallock(sizeof(page));
    *curr = (page){
        1,
        0,
        0,
        0,
        0,
        1,
        MMU_END + (tmp-PAGES_OFF)*PAGE_SIZE
    };
    return curr;
}

void free_page(page *addr){
    hfree(page_heap,(void *)((addr->real-MMU_END)/PAGE_SIZE +PAGES_OFF));
    kfree(addr);
}

