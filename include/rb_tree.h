#ifndef RB_TREE_H_
#define RB_TREE_H_

#define RB_RED 0
#define RB_BLACK 1

#define null 0


typedef int rb_color;

struct rb_entery_t
{
    int                   key;
    rb_color              color;
    void                  * data;
    struct rb_entery_t    * parent;
    struct rb_entery_t    * left;
    struct rb_entery_t    * right;
    void                  *cash_pointer;
};

#define  rb_e struct rb_entery_t
typedef struct 
{    
    struct rb_entery_t    * root;
    struct rb_entery_t    * leftest;
    struct rb_entery_t    * rightest;
    struct rb_entery_t    * nil;
} rb_t;

void __tree_insert(rb_t *tree, struct rb_entery_t *current);

struct rb_entery_t* rb_find(rb_t *tree,int key);

void rb_insert(rb_t *tree, int key, void *data);

void _rb_insert(rb_t *tree, rb_e *x);

rb_e *rb_next(rb_t *tree, int key);

rb_t* rb_create();

rb_t* _rb_create(rb_t  *tree, rb_e *tmp);

void rb_delete(rb_t *tree, struct rb_entery_t *current);

void __right_turn(rb_t *tree, struct rb_entery_t *current);

void __left_turn(rb_t *tree, struct rb_entery_t *current);

#endif
