#include <mem/mem.h>
#include <writer.h>
#include <data/heap.h>


heap *kernal_heap = null;


void init_mem(){
    kernal_heap = init_heap(kernal_heap,\
            (void *)MEM_BEG,\
            (void *)MEM_END,\
            CASH_SIZE);
}

void *kmallock(size_t size){
    void * a =  hmallock(kernal_heap, size);
    kprint((int)a);
    kprint(" ");
    return a;
}

void kfree(void *addr){
    hfree(kernal_heap, addr);
}
