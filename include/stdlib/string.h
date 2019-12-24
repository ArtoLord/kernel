#ifndef STRING_H_
#define STRING_H_

#include "types.h"

#define string char*

extern size_t strlen(string s);
extern string strcpy(string s1, string s2);
extern string strncpy(string s1, const string s2, size_t n);
extern void* memcpy(void* buf1, const void* buf2, size_t bytes);
extern void* memset(void* buf1, u8 value, size_t bytes);
extern int strcmp(const char* s1, const char* s2);
extern int strncmp(const char* s1, const char* s2, u_int n);
extern char* itoa(int value, char* str);

#endif
