#include <iostream>
#include <stdlib.h>
#include <string>

struct node {
    node* next ;
    node* prev ;
    node* parent ;
    node* child ;
    int val ;
    int degree ;
    bool marked ;
    node(int val) : val(val), next(this), prev(this), parent(NULL), child(NULL), degree(0), marked(false) {} ;
    void setParent(node*) ;
    struct node* searchTree(int) ;
};

void node::setParent(node* parent) {
    if(this->parent == parent) return ;
    this->parent = parent ;
    this->next->setParent(parent) ;
}

struct node* node::searchTree(int val) {
    struct node* curr = this ;
    while(true) {
        if(curr->val == val) return curr ;
        if(curr->child) {
            struct node* r_val = curr->child->searchTree(val) ;
            if(r_val) return r_val ;
        }
        curr = curr->next ;
        if(curr == this) return 0 ;
    }
}

class Heap {
    struct node* heap ;
    struct node* node_list[2000000] = {NULL};
public:
    Heap() : heap(NULL) {} ;
    void insert(int) ;
    struct node* merge(struct node*, struct node*) ;
    int extract_min() ;
    void find_min () ;
    struct node* search (int) ;
    void display() ;
    void decrease_key(int, int) ;
};

void Heap::insert(int val) {
    if (!heap) {
        heap = new struct node(val) ;
        node_list[val] = heap ;
        return ;
    }
    struct node* temp = new struct node(val) ;
    node_list[val] = temp ;
    this->heap = merge(this->heap, temp) ;
}

struct node* Heap::search(int val) {
    return this->heap->searchTree(val) ;
}

void Heap::decrease_key(int from, int to) {
    struct node* target = node_list[from] ;
    target->val = to ;
    if(target->val < target->parent->val) {
        // break!
        if(target->parent->marked)
        target->setParent(NULL) ;
    }
}

// h1 is modified
struct node* Heap::merge(struct node* h1, struct node* h2) {
    if(h1 == NULL) return h2 ;
    if(h2 == NULL) return h1 ;

    if (h2->val < h1->val) {
        struct node* temp = h2 ;
        h2 = h1 ;
        h1 = temp ;
    }
    struct node* heap_n = h1->next ;
    struct node* h_p = h2->prev ;
    h1->next = h2 ;
    h2->prev = h1 ;
    heap_n->prev = h_p ;
    h_p->next = heap_n ;
    return h1 ;
}

void Heap::display() {
    struct node* curr = heap ;
    while(true) {
        std::cout << curr->val << std::endl ;
        curr = curr->next ;
        if(curr == heap) break ;
    }
}

int Heap::extract_min() {
    // delete head
    int min = this->heap->val ;
    this->node_list[min] = NULL ;
    if(this->heap->child)
        this->heap->child->setParent(NULL) ;
    this->heap = merge(this->heap, this->heap->child) ;
    struct node* prev = this->heap->prev ;
    struct node* next = this->heap->next ;
    prev->next = next ;
    next->prev = prev ;
    // merge kids
    this->heap = next ;


    // make an array
    int degree = 0 ;
    struct node* sizes[32] = {NULL} ;
    struct node* curr = this->heap ;
    // loop
    while(true) {
        degree = curr->degree ;
        // fill up list
        if(sizes[degree] == NULL) {
            sizes[degree] = curr ;
            curr = curr->next ;
            continue ;
        }
        else {
            struct node *swap = sizes[degree] ;
            // break condition
            if(swap == curr) break ;
            // check which one is smaller
            if (swap->val >= curr->val) {
                struct node* next = swap->next ;
                struct node* prev = swap->prev ;
                swap->next = swap ;
                swap->prev = swap ;
                // to merge, first set the going-to-merge's parents
                swap->setParent(curr) ;
                curr->child = merge(curr->child, swap);
                curr->degree += 1 ;

                prev->next = next;
                next->prev = prev;
                curr = curr->next ;
            }
            else {
                struct node* next = curr->next ;
                struct node* prev = curr->prev ;
                curr->next = curr ;
                curr->prev = curr ;
                // same
                curr->setParent(swap) ;
                swap->child = merge(swap->child, curr);
                swap->degree += 1 ;
                // extra steps to allow it to move forward
                prev->next = next ;
                next->prev = prev ;
                curr = next ;
            }
            sizes[degree] = NULL ;
        }
    }
    return min ;
}

int main() {
    Heap *h = new Heap() ;
    h->insert(5) ;
    h->insert(9) ;
    h->insert(2) ;
    h->insert(8) ;
    h->insert(3) ;
    h->insert(1) ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
}