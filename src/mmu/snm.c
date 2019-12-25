#include <mmu/snm.h>
#include <mem/mem.h>
#include <stdlib/deque.h>

int abs(int a){
    if (a<0) return -a;
    return a;
}

void make_set(snm *s, u32 ind, snm_head *next){
    snm_head *curr = kmallock(sizeof(snm_head));
    s->parents[ind] = -((long)curr);
    s->ranks[ind] = 0;
    deque *childs_all  = new_deque(null);
    deque *dfs = new_deque(null);
    deque *childs = new_deque(null);
    *curr = (snm_head){
        ind,
        childs,
        dfs,
        childs_all,
        true,
        next,
        1
    };
    push_front(dfs,ind);
}


snm *init_snm(u32 *frames, u32 nframes){
    snm *curr = (snm *)kmallock(sizeof(snm));
    u32 *parents = (u32 *) kmallock(sizeof(int)*nframes);
    snm_head *heads = (snm_head *)kmallock(sizeof(snm_head)*nframes);
    u32 * ranks = (u32 *) kmallock(sizeof(u32)*nframes);
    *curr =(snm){
            frames,
            nframes,
            heads,
            parents,
            ranks
    };
    make_set(curr,nframes-1,null);
    for (int i = nframes - 2; i>=0;i++){
        make_set(curr, i, &curr->heads[i+1]);
    }
    return curr; 
}

int max(int a, int b){
    if (a>b) return a;
    return b;
}

void union_snm(snm *s, snm_head *h1, snm_head *h2){
    if (h2->size < 4){
        snm_head *t = h1; h1 = h2; h2 = t;
    }
    if (h1->size < 4){
        s->parents[h1->ind] = h2->ind;
        s->ranks[h1->ind] = 0;
        push_front(h2->childs, h1->ind);
        for (deque_e *i = h1->childs_all->front; i!=null; i = i->next){
            u32 curr = *(u32 *)GET_DEQUE_DATA(i);
            s->parents[curr] = h2->ind;
            s->ranks[curr] = 0;
        }
        push_back_deque(h2->dfs, h1->dfs);
        push_back_deque(h2->childs, h1->childs); 
        return;
    }
    if (s->ranks[h1->ind]<s->ranks[h2->ind]){ 
        snm_head *t = h1; h1 = h2; h2 = t;
    }
    s->parents[h2->ind] = h1->ind;
    if (s->ranks[h2->ind] == s->ranks[h1->ind]){
        s->ranks[h1->ind]++;
    }
    push_front(h1->childs, h2->ind);
    push_front(h1->childs_all, h2->ind);
    pop_front(h1->dfs);
    push_front_deque(h1->dfs, h2->dfs);
    push_front(h1->dfs, h1->ind);
    clear(h2->childs_all); 
}


void relink(snm *s, u32 x){
};
