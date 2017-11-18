#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>

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
    bool isMarked() ;
};

void node::setParent(node* parent) {
    if(this->parent == parent) return ;
    this->parent = parent ;
    this->next->setParent(parent) ;
}
bool node::isMarked() {
    return this->marked ;
}

class Heap {
    struct node* heap ;
    struct node** node_list ;
	// store for array
	int size ;
public:
    Heap(int size) : heap(NULL), size(size) { node_list = new struct node*[size * sizeof(struct node)] ; } ;
    void insert(int, int) ;
    struct node* merge(struct node*, struct node*) ;
    int extract_min() ;
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
//        return ;
    }
    else {
        if(n->parent->child == n)
            n->parent->child = n->next ;
    }
    n->parent->degree -= 1 ;
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
void Heap::decreaseKey(int elem, int to) {
    struct node* target = node_list[elem] ;
    if (!target) return ;
    if (target->key < to) return ;
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

struct node* Heap::merge(struct node* h1, struct node* h2) {
    if(h1 == NULL) return h2 ;
    if(h2 == NULL) return h1 ;

    if (h2->key < h1->key) {
        struct node* temp = h2 ;
        h2 = h1 ;
        h1 = temp ;
    }
    struct node* h_n = h1->next ;
    struct node* h_p = h2->prev ;
    h1->next = h2 ;
    h2->prev = h1 ;
    h_n->prev = h_p ;
    h_p->next = h_n ;
    return h1 ;
}

int Heap::extract_min() {
    // delete head
    int count = 0 ;
    int min = this->heap->elem ;
    this->node_list[min] = NULL ;
    if(this->heap->child) {
        this->heap->child->setParent(NULL);
        count += this->heap->degree ;
    }
    this->heap = merge(this->heap, this->heap->child) ;
    struct node* prev = this->heap->prev ;
    struct node* next = this->heap->next ;
    prev->next = next ;
    next->prev = prev ;
    // merge kids
	delete(this->heap) ;
    this->heap = next ;


    int degree = 0 ;
    // we have to null initialize this array because we actively check whether something is NULL
    struct node* sizes[(int)ceil(log2(this->size)) + 1] = {NULL};
    struct node* curr = this->heap ;
    // loop
    while(true) {
        degree = curr->degree ;
        // fill up list
        if(!sizes[degree]) {
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
                // swap->setParent(curr) ;
				swap->parent = curr ;
                count += 1 ;
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

//                curr->setParent(swap) ;
				curr->parent = swap ;
                count += 1 ;
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

        this->heap = this->heap->next ;
    }
	//delete(sizes) ;
    return count ;
}

int main() {

    Heap *h = new Heap(0) ;
    std::cin.sync_with_stdio(false) ;
    std::cout.sync_with_stdio(false) ;
    std::ios_base::sync_with_stdio(false) ;
    std::string line ;
    std::string full_op, remainder ;
    std::string a1, a2 ;
    std::ifstream f ;
    f.open("/home/vinit/Uni/NTIN066/fibheap/temp2") ;
    if(!f) {
        std::cout << "fail" ;
    }
    int steps_sum = 0 ;
    int count = 0 ;
    int size = 0 ;
    int n1, n2 ;
    while(std::getline(std::cin, line)) {
        char *c_str = (char*)line.c_str() ;
        char *op = strtok(c_str, " ") ;
        if(strcmp(op, "#") == 0) {
            if(count != 0)
                std::cout << steps_sum / count << "\t" << size << std::endl ;
            delete(h) ;
            size = atoi(strtok(NULL, " ")) ;
//            size = atoi(line.substr(2, -1).c_str()) ;
            h = new Heap(size) ;
            steps_sum = 0 ;
            count = 0 ;
        }
 //           full_op = line.substr(0, 3).c_str() ;
        else if(strcmp(op, "INS") == 0) {
            n1 = atoi(strtok(NULL, " ")) ;
            n2 = atoi(strtok(NULL, " ")) ;
            h->insert(n2, n1) ;
        }
        else if(strcmp(op, "DEC") == 0) {
            n1 = atoi(strtok(NULL, " ")) ;
            n2 = atoi(strtok(NULL, " ")) ;
            h->decreaseKey(n1, n2) ;
        }
        else {
            steps_sum += h->extract_min();
            count += 1 ;
        }
    }
}
