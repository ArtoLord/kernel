#include <data/heap.h>




rb_e_cash * get_used(
        heap *h,\
        size_t size,\
        size_t addr,\
        mem_block_t *prev,\
        mem_block_t *next){
    GET_HEAP(h);
    mem_block_cash *main = (mem_block_cash *) cash_allock(mem_holder);
    rb_e_cash *main_rb = (rb_e_cash *) cash_allock(rb_holder);
    main->entry = (mem_block_t){
        addr,\
        size,\
        true,\
        prev,\
        next,\
        main
    };
    main_rb->entry = (rb_e){
        size,
        RB_BLACK,
        &main->entry,
        tree_used->nil,
        tree_used->nil,
        tree_used->nil,
        main_rb
    };
 
    return  main_rb;
}

rb_e_cash * get_unused(
        heap *h,\
        size_t size,\
        size_t addr,\
        mem_block_t *prev,\
        mem_block_t *next){
    GET_HEAP(h);
    mem_block_cash *main = (mem_block_cash *)cash_allock(mem_holder);
    rb_e_cash *main_rb = (rb_e_cash *)cash_allock(rb_holder);
    main->entry = (mem_block_t){
        addr,
        size,
        false,
        prev,
        next,
        main
    };
    main_rb->entry = (rb_e){
        addr,
        RB_BLACK,
        &main->entry,
        tree_unused->nil,
        tree_unused->nil,
        tree_unused->nil,
        main_rb
    };
    return main_rb;
}
rb_e_cash * get_used_to( heap *h,size_t addr, rb_e *unused){
    GET_HEAP(h);
    rb_e_cash *main_rb = (rb_e_cash *)cash_allock(rb_holder);

    main_rb->entry = (rb_e){
        addr,
        RB_BLACK,
        unused,
        tree_used->nil,
        tree_used->nil,
        tree_used->nil,
        main_rb
    };
    return main_rb;
}
heap *init_heap(heap *h,void *beg, void *end, size_t cash_size){
    rb_t *tree_used = (rb_t *)(beg + TREE_U_O);
    rb_t *tree_unused = (rb_t *)(beg + TREE_UN_O);
    rb_t *tree_used_to_unused = (rb_t *)(beg + TREE_U_T_UN_O);
    cash_holder *mem_holder = (cash_holder *)(beg + M_HOLDER_O);
    cash_holder *rb_holder = (cash_holder *)(beg + RB_HOLDER_O);
    init_cash(sizeof(mem_block_cash),\
            cash_size,\
            (size_t)(beg+CASH_O),\
            mem_holder\
            );
    init_cash(sizeof(rb_e_cash),\
            cash_size,\
            (size_t)(beg + CASH_O + sizeof(mem_block_cash)*cash_size),\
            rb_holder\
            );

    rb_e_cash *tmp = (rb_e_cash *)cash_allock(rb_holder);
    _rb_create(tree_unused,&tmp->entry);
    _rb_create(tree_used, &tmp->entry);
    _rb_create(tree_used_to_unused, &tmp->entry);
    *h = (heap){
        tree_used,
        tree_unused,
        tree_used_to_unused,
        mem_holder,
        rb_holder
    };
    rb_e *curr = &get_unused(h,\
            (size_t)(beg+CASH_O+\
            sizeof(mem_block_cash)*cash_size+\
            sizeof(rb_e_cash)*cash_size),\
            end-\
            (beg+CASH_O+\
            sizeof(mem_block_cash)*cash_size+\
            sizeof(rb_e_cash)*cash_size)\
            , null, null)->entry;


    rb_e *used_to = null;
    mem_block_t *curr_block = curr->data;
    used_to = &get_used_to(h, curr_block->addr, curr)->entry;
    
    _rb_insert(tree_unused, curr);
    _rb_insert(tree_used_to_unused, used_to);
    return h;
}

void *hmallock(heap *h,size_t size){
    GET_HEAP(h);
    //assert(size>0);
    rb_e * curr = rb_next(tree_unused ,size);
    //assert(curr==tree_unused->nil);
    mem_block_t *curr_block = curr->data;
    if (curr_block->size==size){
        curr_block->used = true;
        rb_delete(tree_unused,curr);
        _rb_insert(tree_used,curr);
        return (void *)curr_block->addr;
    }

    rb_e *curr_to = rb_find(
            tree_used_to_unused,\
            curr_block->addr
            );
    rb_delete(tree_unused,curr);
    rb_delete(tree_used_to_unused, curr_to);

    rb_e_cash * used_cash = get_used(h,
            curr_block->addr,\
            size,\
            curr_block->prev,\
            curr_block->next
            );
    rb_e *used = &used_cash->entry;

    rb_e *unused =&get_unused(h,
            curr_block->addr+size,\
            curr_block->size-size,\
            used->data,\
            ((mem_block_t *)used->data)->next
            )->entry;

    ((mem_block_t *)used->data)->next = unused->data;
    if (((mem_block_t *)used->data)->prev != null)
        ((mem_block_t *)used->data)->prev->next = used->data;

    if (((mem_block_t *)unused->data)->next != null)
        ((mem_block_t *)unused->data)->next->prev = unused->data;


    rb_e *used_to = &get_used_to(h, ((mem_block_t *)unused->data)->addr, unused)->entry;
    _rb_insert(tree_used, used);
   _rb_insert(tree_unused, unused);
    _rb_insert(tree_used_to_unused, used_to);

    cash_free(rb_holder, curr->cash_pointer);
    cash_free(mem_holder, curr_block->cash_pointer);
    return (void *)curr_block->addr;
}

void hfree(heap *h, void *addr){
    GET_HEAP(h);

    rb_e *curr = rb_find(tree_used, (int)addr);
    //assert(curr==tree_used->nil);
 
    mem_block_t *curr_block = curr->data;
    curr_block->used = false;

    if (curr_block->prev!=null && !curr_block->prev->used){
        rb_e *prev_to = rb_find(tree_used_to_unused, curr_block->prev->addr);
        //assert(prev_to==tree_used->nil);
        rb_delete(tree_used_to_unused, prev_to);
        rb_e *prev = prev_to->data;
        mem_block_t *prev_block = prev->data;

        cash_free(rb_holder, prev_to->cash_pointer);
        rb_delete(tree_unused, prev);


        curr_block->prev = prev_block->prev;
        curr_block->size +=prev_block->size;
        prev_block->prev->next = curr_block; 
        curr_block->addr = prev_block->addr;
        cash_free(mem_holder, prev_block->cash_pointer);
        cash_free(rb_holder, prev->cash_pointer);


    }
    if (curr_block->next!=null && !curr_block->next->used){
        rb_e *next_to = rb_find(tree_used_to_unused, curr_block->next->addr);
        //assert(next_to==tree_used->nil);
        rb_delete(tree_used_to_unused, next_to);
        rb_e *next = next_to->data;
        mem_block_t *next_block = next->data;

        cash_free(rb_holder, next_to->cash_pointer);
        rb_delete(tree_unused, next);



        curr_block->next = next_block->next;
        curr_block->size +=next_block->size;
        next_block->next->prev = curr_block; 
        cash_free(mem_holder, next_block->cash_pointer);
        cash_free(rb_holder, next->cash_pointer);

    }

    rb_e *curr_to = &get_used_to(h, ((mem_block_t *)curr->data)->addr, curr)->entry;

    _rb_insert(tree_used_to_unused,curr_to);
    rb_delete(tree_used, curr);

    curr->left = tree_unused->nil;
    curr->right = tree_unused->nil;
    curr->parent = tree_unused->nil;
    curr->key = curr_block->addr;
    _rb_insert(tree_unused,curr);
}