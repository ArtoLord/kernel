#include "rb_tree.h"
#include "stdlib.h"
#define rb_e struct rb_entery_t


void __rb_delete_fixup(rb_t *tree, rb_e *x);


rb_t* _rb_create(rb_t *tree, rb_e *tmp){
    tmp->color = RB_BLACK;
    tmp->left = tmp;
    tmp->right = tmp;
    tmp->parent = tmp;
    tmp->data = null;
    tmp->key = 0;

    *tree = (rb_t){tmp, tmp, tmp,tmp};
    return tree;
}
/*
rb_t* rb_create(){
    rb_e *tmp =malloc(sizeof(rb_e));
    rb_t *tree = malloc(sizeof(rb_t)); 
    return rb_create(tree,tmp);
}
*/
void __tree_insert(rb_t *tree, rb_e *current){
    rb_e *tmp = tree->nil;
    rb_e *this = tree->root;
    while (this != tree->nil){
        tmp = this;
        if (current->key < this->key)
            this = this->left;
        else 
            this = this->right;
    }
    current->parent = tmp;
    if (tmp == tree->nil) tree->root = current;
    else if (current->key<tmp->key){
        tmp->left = current;
    }
    else{
        tmp->right = current;
    }
}

void __left_turn(rb_t *tree, rb_e *x){
    rb_e *y = x->right;
    x->right = y->left;
    if (y->left !=tree->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil) 
        tree->root = y;
    else if (x == x->parent->left) 
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}



void __right_turn(rb_t *tree, rb_e *x){
    rb_e *y = x->left;
    x->left = y->right;
    if (y->right !=tree->nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil) 
        tree->root = y;
    else if (x == x->parent->right) 
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}


void __update_leaves(rb_t *tree){
    rb_e *curr = tree->root;
    while (curr != tree->nil)
        curr = curr->left;
    tree->leftest = curr;
    curr = tree->root;
    while (curr != tree->nil)
        curr = curr->right;
    tree->rightest = curr;
}

void _rb_insert(rb_t *tree, rb_e *x){
    __tree_insert(tree, x);
    
    while(x!=tree->root && x->parent->color==RB_RED){
        if (x->parent==x->parent->parent->left){
            rb_e *y = x->parent->parent->right;
            if (y->color == RB_RED){
                x->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                x->parent->parent->color = RB_RED;
                x= x->parent->parent;
            }
            else{
                if (x == x->parent->right){
                    x = x->parent;
                    __left_turn(tree, x);
                }
                x->parent->color = RB_BLACK;
                x->parent->parent->color = RB_RED;
                __right_turn(tree,x->parent->parent);
            }
        }
        else{
            rb_e *y = x->parent->parent->left;
            if (y->color == RB_RED){
                x->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                x->parent->parent->color = RB_RED;
                x= x->parent->parent;
            }
            else{
                if (x == x->parent->left){
                    x = x->parent;
                    __right_turn(tree, x);
                }
                x->parent->color = RB_BLACK;
                x->parent->parent->color = RB_RED;
                __left_turn(tree,x->parent->parent);
            }
        }
    }
    tree->root->color = RB_BLACK;
}



/*
void rb_insert(rb_t *tree, int key, void *data){
    rb_e *x = malloc(sizeof(rb_e));
    *x = (rb_e){
        key,
        RB_RED,
        data,
        tree->nil,
        tree->nil,
        tree->nil
    };
    _rb_insert(tree,x); 
}

*/
rb_e *__tree_min(rb_t *tree, rb_e *x){
    while (x->left != tree->nil){
        x = x->left;
    }
    return x;
}

rb_e* __rb_next(rb_t *tree, rb_e *x){
    if (x->right!=tree->nil){
        return __tree_min(tree,x->right);
    }
    rb_e *y = x->parent;
    while(y != tree->nil && x==y->right){
        x = y;
        y = y->parent;
    }
    return y;
}

void rb_delete(rb_t *tree, rb_e *z){
    rb_e *y = tree->nil, *x = tree->nil;
    if (x==null || z==tree->nil) return;
    if (z->left==tree->nil  || z->right == tree->nil){
        y = z;
    }
    else{
        y = __rb_next(tree,z);
    }
    if (y->left != tree->nil){
        x = y->left;
    }
    else{
        x = y->right;
    }
    x->parent = y->parent;

    if (y->parent== tree->nil){
        tree->root = x;
    }
        else if (y == y->parent->left){
            y->parent->left = x;
        }
        else y->parent->right = x;
    if (y!=z){
        z->key = y->key;
        z->data = y->data;
        z->cash_pointer = y->cash_pointer;
    }
    if (y->color==RB_BLACK){
        __rb_delete_fixup(tree,x);
    }
}

void __rb_delete_fixup(rb_t *tree, rb_e *x){
    while (x != tree->root && x->color == RB_BLACK){
        if (x==x->parent->left){
            rb_e *w = x->parent->right;
            if (w->color==RB_RED){
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                __left_turn(tree,x->parent);
                w = x->parent->right;
            }
            if (w->left->color==RB_BLACK && w->right->color==RB_BLACK){
                w->color = RB_RED;
                x = x->parent;
            }
            else{
                if (w->right->color == RB_BLACK){
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    __right_turn(tree,w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                __left_turn(tree,x->parent);
                x = tree->root;
            }
        }
        else{
            rb_e *w = x->parent->left;
            if (w->color==RB_RED){
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                __right_turn(tree,x->parent);
                w = x->parent->left;
            }
            if (w->right->color==RB_BLACK && w->left->color==RB_BLACK){
                w->color = RB_RED;
                x = x->parent;
            }
            else {
                if (w->left->color == RB_BLACK){
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    __left_turn(tree,w);
                    w = x->parent->left;
                } 
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                __right_turn(tree,x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RB_BLACK;
}

rb_e* rb_find(rb_t *tree, int key){
    rb_e *x = tree->root;
    while(x!=tree->nil && key!= x->key){
        if (key<x->key) x = x->left;
        else x = x->right;
    }
    if (x==tree->nil) return null;
    return x;
}

rb_e *rb_next(rb_t *tree, int key){
    rb_e *curr = tree->root, *succ = tree->nil;
    while (curr!=tree->nil){
        if (curr->key>key){
            succ = curr;
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return succ;
}


