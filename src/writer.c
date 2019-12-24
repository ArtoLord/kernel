#include "writer.h"

char * videomem  = (char *)0xB8000;

int current_pos = 0;
size_t current_buffer_size = BUFFER_SIZE;
char  *string_buffer = (char *)0x10000000;
char  *type_buffer = (char *)(0x10000000+sizeof(char)*BUFFER_SIZE);
int scroll_pos = 0;


void _write_to_buffer(char s, char type);

void writer_init(){
    char * curr_str = kmallock(sizeof(char)*current_buffer_size*2);
        char * curr_type = kmallock(sizeof(char)*current_buffer_size*2);
        for (int i = 0; i < current_buffer_size*2;i++){
            string_buffer[i] = 0;
            type_buffer[i] = 0;
        }
        for (int i = 0;i<current_buffer_size;i++){
            curr_str[i] = string_buffer[i];
            curr_type[i] = type_buffer[i];
        }
        string_buffer = curr_str;
        type_buffer = curr_type;
        current_buffer_size *= 2;
    for (int i = 0;i<BUFFER_SIZE;i++){
        string_buffer[i] = 0;
        type_buffer[i] = 0;
    }
}


int min(int a, int b){
    if (a<b){
    return a;
    }
    return b;
}

extern void kprint_str(string str){
    int len = strlen(str);
    for(int i = 0;i<len;i++){
        _write_to_buffer(str[i], TYPE_STANDART);
    }
}

void kprint_int(int ch){
    char s[10];
    itoa(ch,s);
    kprint(s);
}


void kprint_str_str(string str, string types){
    int len = strlen(str);
    for(int i = 0;i<len; i++){
        _write_to_buffer(str[i], types[i]);
    }
}

void _write_to_buffer(char s, char type){
    if (s!='\n'){
        string_buffer[current_pos] = s;
        type_buffer[current_pos] = type;
    }
    if (s=='\n'){
        current_pos += VIDEO_SCREEN_WIDTH - (current_pos % VIDEO_SCREEN_WIDTH);
    }
    if (current_pos>=current_buffer_size-1){
        char * curr_str = kmallock(sizeof(char)*current_buffer_size*2);
        char * curr_type = kmallock(sizeof(char)*current_buffer_size*2);
        for (int i = 0; i < current_buffer_size*2;i++){
            string_buffer[i] = 0;
            type_buffer[i] = 0;
        }
        for (int i = 0;i<current_buffer_size;i++){
            curr_str[i] = string_buffer[i];
            curr_type[i] = type_buffer[i];
        }
        kfree(string_buffer);
        kfree(type_buffer);
        string_buffer = curr_str;
        type_buffer = curr_type;
        current_buffer_size *= 2;
    }
    if (s!='\n')
        current_pos++;
    display();
}

void kprint_str_ch(string str, char type){
    int len = strlen(str);
    for(int i = 0;i<len;i++){
        _write_to_buffer(str[i],type);
    }
}

void kpanic(string str){
    kprint_t(str, TYPE_PANIC);
    //asm_hlt();
}

void kwarning(string str){
    kprint_t(str, TYPE_WARNING);
}

void kprint_ch(char ch){
    _write_to_buffer(ch, TYPE_STANDART);
}

void kprint_ch_ch(char ch, char type){
    _write_to_buffer(ch, type);
}



void kwriteln(){
    _write_to_buffer('\n', TYPE_STANDART);
}

void display(){
    for(int i = 0; i<VIDEO_SIZE; i++){
        videomem[i*2] = ' ';
        videomem[i*2+1] = 0x07;
    }
    for(int i = scroll_pos; i<min(scroll_pos+VIDEO_SIZE,current_pos); i++)
    {
        videomem[2*(i-scroll_pos)] = string_buffer[i];
        videomem[2*(i-scroll_pos)+1] = type_buffer[i];
    }
}

void kscroll(int lines){
    scroll_pos +=VIDEO_SCREEN_WIDTH;
};

