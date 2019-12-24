#ifndef WRITER_H_
#define WRITER_H_

#include "stdlib/string.h"
#include "mem/mem.h"


#define BUFFER_SIZE  500

#define TYPE_STANDART  0x07

#define TYPE_PANIC  0x04

/*
 *colors:
 * 0x01 green
 * 0x02 blue
 * 0x03 light blue
 * 0x04 dark red
 * 0x05 lil
 * 0x06 orange
 * 0x08 gray
 * 0x09 dark blue
 * 0x0a green
 * 0x0b ==0x03
 */

#define TYPE_WARNING  0x06

#define VIDEO_SCREEN_WIDTH 80

#define VIDEO_SCREEN_HEIGHT 25

#define VIDEO_SIZE 80*25


#define kprint(X)\
      _Generic((X),\
          int: kprint_int,\
          string: kprint_str,\
          char: kprint_ch ,\
          const string: kprint_str \
          )(X)
  
  #define kprint_t(X,Y)\
      _Generic((X),\
              string: _Generic((Y),\
                  char: kprint_str_ch,\
                  string: kprint_str_str,\
                  int: kprint_str_ch\
                  ),\
              char:   kprint_ch_ch \
              )(X,Y)

typedef struct char_t{
    char ch;
    char type;
}__attribute__((packed)) char_u;

void display();

void kprint_str(string str);

void writer_init();

void kprint_str_str(string str, string types);

void kprint_str_ch(string str, char type);

void kprint_ch(char ch);

void kprint_int(int in);

void kpanic(string str);

void kwarning(string str);

void kprint_ch_ch(char ch, char type);

void kwriteln();

void kscroll(int lines);



#endif
