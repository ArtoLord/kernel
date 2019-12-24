#ifndef TYPES_H_
#define TYPES_H_

typedef int bool;
#define true 1
#define false 0
#define null 0

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned int u_long;

typedef unsigned long size_t;

#define HIGH_WORD(addr) ((addr & 0xffff0000) >> 16)
#define LOW_WORD(addr) ((addr & 0xffff))
#define LOW_BYTE(addr) ((addr & 0x00ff))
#define HIGH_BYTE(addr) ((addr & 0xff00) >> 8)

//typedef size_t* va_list;

#define va_start(l, a) (l = (void*)((size_t)&a) + sizeof(a))
#define va_end(l) (l = (void*)0)
#define va_arg(l, s) (*(s*)(l++))

#define attribute(a) __attribute__((a))

#endif 
