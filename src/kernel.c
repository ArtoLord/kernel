#include "writer.h"
#include "mem/mem.h"
#include "arch/descriptor_tables.h"
#include <arch/timer.h>
#include <stdlib/deque.h>
void kmain(void)
{    
    init_descriptor_tables();   
    kprint("HEY");
    init_mem();

    writer_init();
    kprint("[KERNEL]  started\n");
    kprint((int)kmallock(sizeof(int)));
    kprint(" ");
    kprint((int)kmallock(sizeof(int)));
    kprint(" ");
    kprint((int)kmallock(sizeof(int)));
    kprint(" ");
    kprint((int)kmallock(sizeof(int)));
    //deque * d = new_deque();
    int a = 1;
    string b = "Hello";
    char c = 'H';

    /*
    push_back(d,a);
    push_back(d,b);
    push_back(d, c);
    while(d->front!=null){
        kprint(front(d));
        pop_back(d);
    }
    clear(d);
    */
    kscroll(1000);
    while(1);
	return;
}
