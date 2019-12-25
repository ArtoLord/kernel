#include <mmu/page.h>
#include <data/heap.h>
#include <mem/mem.h>
#include <writer.h>

#define page_num  1000000
#define sqrt_page_num  1000

u32 *frames;
u32 nframes;

u32 *blocks;

page_directory * current_dir;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

void init_mmu(){
    frames = (u32 *)kmallock(sizeof(u32)*page_num/32+1);
    nframes = page_num;
    
    blocks = (u32 *)kmallock(sqrt_page_num*sizeof(u32));
    for (int i = 0; i<sqrt_page_num;i++){
        blocks[i] = 0;
    }
    for (int i = 0; i<page_num/32+1; i++){
        frames[i] = 0;
    }

    page_directory *kernal_dir \
        = (page_directory *)kmallock_a(sizeof(page_directory));

    for (int i = 0;i<1024;i++){
        kernal_dir->tablesPhysical[i] = null;
        kernal_dir->tables[i] = null;
    }
    for (int i = 0; i*0x1000<MEM_END;i++){
        page * p = get_page(i*0x1000,1,kernal_dir);
        allock_frame(p,1,1);
    }


    register_interrupt_handler(14, page_fault);
    for (int i = 0;i<1024;i++){
        if (kernal_dir->tablesPhysical[i]!=null){
            kprint(i);
            kprint(" ");
        }
    }
    switch_page_dir(kernal_dir);
}

void switch_page_dir(page_directory *dir)
{
   current_dir = dir;
   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   u32 cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}


static void set_frame(u32 frame_addr)
{
   u32 frame = frame_addr/0x1000;
   blocks[frame/sqrt_page_num]++;
   u32 idx = INDEX_FROM_BIT(frame);
   u32 off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32 frame_addr)
{
   u32 frame = frame_addr/0x1000;
   blocks[frame/sqrt_page_num]--;
   u32 idx = INDEX_FROM_BIT(frame);
   u32 off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static u32 test_frame(u32 frame_addr)
{
   u32 frame = frame_addr/0x1000;
   u32 idx = INDEX_FROM_BIT(frame);
   u32 off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

static u32 first_frame(){
    for (int i = 0; i<sqrt_page_num; i++){
        if (blocks[i]<sqrt_page_num){
            for (int j = i*sqrt_page_num;j<i*(sqrt_page_num+1);j++){
                if(!test_frame(j)){
                    return j;
                }
            }
        }
    }
    return -1;
}

page * allock_frame(page * p, bool is_kernal, bool is_super){
    if(p->frame==null){
        return p;
    }
    u32 curr= first_frame();
    if (curr==-1){
        kpanic("No more frames");
    }
    set_frame(curr=curr*0x1000);
    p->frame = curr;
    p->present = true;
    p->rw  = (is_super)?1:0;
    p->supervisor = (is_kernal)?0:1;
    p->frame =curr;
    return p; 
}

void free_frame(page *p){
    if (p->frame==null) return;
    clear_frame(p->frame);
    p->frame = null;
}



page *get_page(size_t addres, bool make, page_directory *dir){
    addres/=0x1000;
    if (dir->tables[addres/1024]!=null){
        return &dir->tables[addres/1024]->pages[addres%1024];
    }
    if (!make) return null;
    u32 tmp = (u32)kmallock_a(sizeof(page_table));
    dir->tables[addres/1024] = (page_table *)tmp;
    dir->tablesPhysical[addres/1024] = tmp; 
    return &dir->tables[addres/1024]->pages[addres%1024];
}


void page_fault(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   u32 faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   kprint("Page fault! ( ");
   if (present) {kprint("present ");}
   if (rw) {kprint("read-only ");}
   if (us) {kprint("user-mode ");}
   if (reserved) {kprint("reserved ");}
   kprint(") at 0x");
   kprint((int)faulting_address);
   kprint("\n");
   kpanic("Page fault");
}
