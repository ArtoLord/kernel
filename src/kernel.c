#include "writer.h"
#include "mem/mem.h"
#include "arch/descriptor_tables.h"
#include <arch/timer.h>
#include <stdlib/deque.h>
#include <mmu/page.h>
void kmain(void)
{    
    init_descriptor_tables();   
    kprint("[KERNEL] dt inited\n");
    init_mem();
    kprint("[KERNEL] mem inited\n");
    writer_init();
    kprint("[KERNEL]  started\n");
    init_mmu();

    kscroll(1000);
    while(1);
	return;
}
