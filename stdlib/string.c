#include "stdlib/string.h"

#define string char*

extern size_t strlen(string s){
    size_t len = -1;
    while(s[++len]!='\0');
    return len;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

extern string strcpy(string s1, string s2){
    size_t i = -1;
    while(s2[++i]!='\0'){
        s1[i] = s2[i];
    }
    s1[i] = s2[i];
    return s1;
}

extern string strncpy(string s1, const string s2, size_t n){
    bool ist = true;
    for(size_t i  = 0;i<n;i++){
        if (s2[i] == '\0') ist = false;
        if (ist) s1[i] = s2[i];
        else s1[i] = '\0';
    }
    return s1;
}

extern void* memcpy(void* buf1, const void* buf2,   size_t bytes){
   size_t i = 0;
   //cast src and dest to char*
   char *src_char = (char *)buf2;
   char *dest_char = (char *)buf1;
   for (i=0; i<bytes; i++)
      dest_char[i] = src_char[i];
   return (void *)dest_char;
}

extern void* memset(void* buf1, u8 value, size_t bytes){
    string buf = (char *) buf1;
    for (size_t i =0;i<bytes;i++) buf[i] = value;
    return (void *)buf;
}



