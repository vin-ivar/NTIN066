#include <iostream>
#include <stdlib.h>
#include <string>

struct node {
    node* next ;
    node* prev ;
    node* parent ;
    node* child ;
    int key ;
    int elem ;
    int degree ;
    bool marked ;
    node(int key, int elem) : key(key), elem(elem), next(this), prev(this), parent(NULL), child(NULL), degree(0), marked(false) {} ;
    void setParent(node*) ;
    struct node* searchTree(int) ;
    bool isMarked() ;
};

void node::setParent(node* parent) {
    if(this->parent == parent) return ;
    this->parent = parent ;
    this->next->setParent(parent) ;
}

struct node* node::searchTree(int key) {
    struct node* curr = this ;
    while(true) {
        if(curr->key == key) return curr ;
        if(curr->child) {
            struct node* r_key = curr->child->searchTree(key) ;
            if(r_key) return r_key ;
        }
        curr = curr->next ;
        if(curr == this) return 0 ;
    }
}

bool node::isMarked() {
    return this->marked ;
}

class Heap {
    struct node* heap ;
    struct node* node_list[2000000] = {NULL};
public:
    Heap() : heap(NULL) {} ;
    void insert(int, int) ;
    struct node* merge(struct node*, struct node*) ;
    int extract_min() ;
    void find_min () ;
    struct node* search (int) ;
    void display() ;
    void decreaseKey(int, int) ;
    void _fixParent(struct node*) ;
    void _fixSiblings(struct node*) ;
    void _recurseFix(struct node*) ;
};

void Heap::insert(int key, int elem) {
    if (!heap) {
        heap = new struct node(key, elem) ;
        node_list[elem] = heap ;
        return ;
    }
    struct node* temp = new struct node(key, elem) ;
    node_list[elem] = temp ;
    this->heap = merge(this->heap, temp) ;
}

// change parent's child
void Heap::_fixParent(struct node* n) {
    if(!n->parent) return ;
    if(n->next == n) {
        n->parent->child = NULL ;
        return ;
    }
    else {
        if(n->parent->child == n)
            n->parent->child = n->next ;
    }
}

void Heap::_fixSiblings(struct node* n) {
    n->prev->next = n->next ;
    n->next->prev = n->prev ;
    n->next = n ;
    n->prev = n ;
}

void Heap::_recurseFix(struct node * n) {
    n = n->parent ;
    while(true) {
        if(!n->parent) return ;

        if(n->isMarked()) {
            struct node* parent = n->parent ;
            _fixParent(n) ;
            _fixSiblings(n) ;
            n->setParent(NULL) ;
            this->heap = merge(this->heap, n) ;
            n = parent ;
        }
        else {
            n->marked = true ;
            break ;
        }
    }
}

struct node* Heap::search(int key) {
    return this->heap->searchTree(key) ;
}

void Heap::decreaseKey(int elem, int to) {
    struct node* target = node_list[elem] ;
    target->key = to ;
    if(!target->parent) return ;
    if(target->key < target->parent->key) {
        // break!
        _recurseFix(target) ;
        _fixParent(target) ;
        _fixSiblings(target) ;
        target->setParent(NULL) ;
        this->heap = merge(this->heap, target) ;
    }
}

// h1 is modified
struct node* Heap::merge(struct node* h1, struct node* h2) {
    if(h1 == NULL) return h2 ;
    if(h2 == NULL) return h1 ;

    if (h2->key < h1->key) {
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
        std::cout << curr->key << std::endl ;
        curr = curr->next ;
        if(curr == heap) break ;
    }
}

int Heap::extract_min() {
    // delete head
    int min = this->heap->elem ;
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
            if (swap->key >= curr->key) {
                struct node* next = swap->next ;
                struct node* prev = swap->prev ;
                swap->next = swap ;
                swap->prev = swap ;
                // to merge, first set the going-to-merge's parents
                swap->setParent(curr) ;
                curr->child = merge(curr->child, swap) ;
                curr->degree += 1 ;

                prev->next = next;
                next->prev = prev;
                curr = curr->next ;

                // keep heap at L1
                this->heap = curr ;
            }
            else {
                struct node* next = curr->next ;
                struct node* prev = curr->prev ;
                curr->next = curr ;
                curr->prev = curr ;

                curr->setParent(swap) ;
                swap->child = merge(swap->child, curr) ;
                swap->degree += 1 ;

                prev->next = next ;
                next->prev = prev ;
                curr = next ;
                /*
                swap->prev->next = swap->next ;
                swap->next->prev = swap->prev ;
                prev->next = swap ;
                next->prev = swap ;
                swap->prev = prev ;
                swap->next = next ;
                */
                this->heap = swap ;
            }
            sizes[degree] = NULL ;
        }
    }
    // set root
    struct node* min_node = this->heap ;
    this->heap = this->heap->next ;
    while(true) {
        if(this->heap->key < min_node->key)
            min_node = this->heap ;

        if(this->heap == min_node)
            break ;
    }
    return min ;
}

int main() {
    Heap *h = new Heap() ;
    h->insert(5, 1) ;
    h->insert(9, 2) ;
    h->insert(2, 3) ;
    h->insert(8, 4) ;
    h->insert(3, 5) ;
    h->insert(1, 6) ;
    h->insert(10, 7) ;
    h->insert(4, 8) ;
    h->insert(15, 9) ;
    h->extract_min() ;
    h->decreaseKey(2, 3) ;
    h->decreaseKey(1, 1) ;
    h->decreaseKey(4, 2) ;
}