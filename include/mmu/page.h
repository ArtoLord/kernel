#pragma once

#include <stdlib/types.h>
#include <arch/isr.h>
#include <arch/common.h>
#include <mem/mem.h>

#define MMU_BEG  0x400000
#define MMU_END  MEM_BEG + PAGE_NUM + sizeof(mem_block_cash)*PAGE_NUM + sizeof(rb_e_cash)*PAGE_NUM+0x1000

#define PAGES_OFF MEM_BEG + sizeof(mem_block_cash)*PAGE_NUM + sizeof(rb_e_cash)*PAGE_NUM+0x1000

#define PAGE_NUM 1000000
#define PAGE_SIZE 0x1000
typedef struct page_t{
    u32     present     :1;
    u32     rw          :1;
    u32     supervisor  :1;
    u32     accessed    :1;
    u32     dirty       :1;
    u32     unused      :7;
    u32     real       :20;
}page;

typedef struct page_table_t{
    page pages[1024];
} page_table;

typedef struct page_directory_t{
    page_table *tables[1024];
    u32        tablesPhysical[1024];
    u32        physicalAddr;
} page_directory;

void init_mmu();

page * allock_page();

void free_page(page *addr);

page * allock_pages(size_t num);

void   free_pages(page *addr, size_t num);

void swith_page_dir(page_directory *dir);

page *get_page(size_t addres, int make, page_directory *dir);


void page_fault(registers_t regs);
