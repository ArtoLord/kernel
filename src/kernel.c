#include "writer.h"
#include "mem/mem.h"
#include "arch/descriptor_tables.h"
#include <arch/timer.h>
#include <stdlib/deque.h>
void kmain(void)
{    
    init_descriptor_tables();   
    kprint("[KERNEL] dt inited\n");
    init_mem();
    kprint("[KERNEL] mem inited\n");
    writer_init();
    kprint("[KERNEL]  started\n");
    deque * d = new_deque();
    int a = 1;
    int b = 2;
    int c = 3;
    push_back(d, a);
    push_back(d, b);
    push_back(d, c);
    while(d->front != null){
        kprint(*(int *)back(d));
        pop_back(d);
    }
    clear(d);
    kscroll(1000);
    while(1);
	return;
}
