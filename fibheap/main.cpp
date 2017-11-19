#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>

struct node {
    node *next, *prev, *parent, *child ;
    int key, elem, degree ;
    bool marked ;
    node(int key, int elem) : key(key), elem(elem), next(this), prev(this), parent(NULL), child(NULL), degree(0), marked(false) {} ;
    void moveToRoot() ;
    bool isMarked() ;
    // TODO: implement getters and setters
    // is that really necessary?
    // maybe not
};

// set all siblings' parents to null as well
void node::moveToRoot() {
    if(this->parent == NULL) return ;
    this->marked = false ;
    this->parent = NULL ;
    this->next->moveToRoot() ;
}

bool node::isMarked() {
    return this->marked ;
}

class Heap {
    node* heap ;
    node** node_list ;
public:
    Heap(int size) : heap(NULL) { node_list = new node*[size * sizeof(node)] ; } ;
    void insert(int, int) ;
    node* merge(node*, node*) ;
    int extractMin() ;
    void decreaseKey(int, int) ;
    void _fixParent(node*) ;
    void _fixSiblings(node*) ;
    void _recurseFix(node*) ;
};

void Heap::insert(int key, int elem) {
    if (!heap) {
        heap = new node(key, elem) ;
        node_list[elem] = heap ;
        return ;
    }
    node* n = new node(key, elem) ;
    node_list[elem] = n ;
    this->heap = merge(this->heap, n) ;
}

// change parent's child to n's sibling / NULL
void Heap::_fixParent(node* n) {
    if(!n->parent) return ;
    if(n->next == n) {
        n->parent->child = NULL ;
    }
    else {
        if(n->parent->child == n)
            n->parent->child = n->next ;
    }
    n->parent->degree -= 1 ;
}

// im too lazy to write this > 1 times
void Heap::_fixSiblings(node* n) {
    n->prev->next = n->next ;
    n->next->prev = n->prev ;
    n->next = n->prev = n ;
}

// recursively fix all the parents until unmarked / root
// use this for classical, ignore for naive
void Heap::_recurseFix(node* n) {
    // start from the parent, this makes it easier to switch between classical / naive
    n = n->parent ;
    while(true) {
        if(!n->parent) return ;

        if(n->isMarked()) {
            node* parent = n->parent ;
            _fixParent(n) ;
            _fixSiblings(n) ;
            n->moveToRoot() ;
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
    node* target = node_list[elem] ;
    if (!target) return ;
    if (target->key < to) return ;
    target->key = to ;
    // target is root and the new min key, set it to be the heap head
    if(!target->parent) {
        if(target->key < this->heap->key) this->heap = target ;
        return ;
    } ;
    // if target is still larger than the parent do nothing, otherwise:
    if(target->key < target->parent->key) {
        _fixParent(target) ;
        _fixSiblings(target) ;
        this->heap = merge(this->heap, target) ;
        // moveToRoot is after _recurseFix because we need the parent
        // also it doesn't really matter, it just looks weird
        _recurseFix(target) ;
        target->moveToRoot() ;
    }
}

// TODO: consider not having a return and instead assuming first argument needs to be modified
node* Heap::merge(node* h1, node* h2) {
    if(!h1) return h2 ;
    if(!h2) return h1 ;

    // condense the two returns into one swap \o/
    if (h2->key < h1->key) {
        node* temp = h2 ;
        h2 = h1 ;
        h1 = temp ;
    }
    node* next = h1->next ;
    node* prev = h2->prev ;
    h1->next = h2 ;
    h2->prev = h1 ;
    next->prev = prev ;
    prev->next = next ;
    return h1 ;
}

int Heap::extractMin() {
    // delete head
    int count = 0 ;
    int min = this->heap->elem ;
    this->node_list[min] = NULL ;
    if(this->heap->child) {
        this->heap->child->moveToRoot() ;
        count += this->heap->degree ;
    }
    this->heap = merge(this->heap, this->heap->child) ;
    node* prev = this->heap->prev ;
    node* next = this->heap->next ;
    prev->next = next ;
    next->prev = prev ;
    // no more memleaks \o/
	delete(this->heap) ;
    this->heap = next ;


    int degree = 0 ;
    // we have to null initialize this array because we actively check whether something is NULL
    // 32 is a safe size, works for all tests
    // log₂ doesn't work for some reason, probably because it's O(log₂) and not log₂
    node* sizes[32] = {NULL};
    node* curr = this->heap ;
    // loop
    while(true) {
        degree = curr->degree ;
        // fill up list
        if(!sizes[degree]) {
            sizes[degree] = curr ;
            curr = curr->next ;
        }
        else {
            node *swap = sizes[degree] ;
            // break condition
            if(swap == curr) break ;
            // check which one is smaller
            if (swap->key >= curr->key) {
                // swap needs to be attached to curr - this is easier
                _fixSiblings(swap) ;
				swap->parent = curr ;
                curr->degree++ ;
                count++ ;
                curr->child = merge(curr->child, swap) ;
            }
            else {
                // curr needs to be attached to swap
                // swap needs to replace curr
                // S -- o -- o -- C -- o
                // o -- o -- S (C) -- o
                // NOT :
                // S (C) -- o -- o -- o
                swap->next->prev = swap->prev ;
                swap->prev->next = swap->next ;

                // this is really ugly
                // you can't "fix" swap without potentially "fixing" its sibling
                // which could be curr
                if(curr->next == curr) {
                    swap->next = swap->prev = swap ;
                }
                // best case, it's not curr, in which case attach swap where it is
                else {
                    curr->next->prev = swap;
                    curr->prev->next = swap;
                    swap->next = curr->next;
                    swap->prev = curr->prev;
                }
                // maybe make this less ugly so i can use _fixSiblings
                curr->next = curr->prev = curr ;
				curr->parent = swap ;
                swap->degree += 1;
                count++;
                swap->child = merge(swap->child, curr) ;

                // keep the curr train going
                curr = swap->next ;
            }
            sizes[degree] = NULL ;
        }
    }
    // set root
	this->heap = curr ;
    node* min_node = this->heap ;
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
    std::ios_base::sync_with_stdio(false) ;
    std::string line ;
    long steps_sum = 0 ;
    long count = 0 ;
    int size = 0 ;
    int n1, n2 ;
	char *op, *c_str ;
	bool flag = false ;
    while(std::getline(std::cin, line)) {
        c_str = (char*)line.c_str() ;
        op = strtok(c_str, " ") ;
        if(strcmp(op, "#") == 0) {
            if(count != 0)
                std::cout << (double) steps_sum / count << "\t" << size << std::endl ;
            delete(h) ;
            size = atoi(strtok(NULL, " ")) ;
            h = new Heap(size) ;
            steps_sum = 0 ;
            count = 0 ;
        }
        else if(strcmp(op, "INS") == 0) {
            n1 = atoi(strtok(NULL, " ")) ;
            n2 = atoi(strtok(NULL, " ")) ;
            // order is the other way round
            h->insert(n2, n1) ;
        }
        else if(strcmp(op, "DEC") == 0) {
            n1 = atoi(strtok(NULL, " ")) ;
            n2 = atoi(strtok(NULL, " ")) ;
            h->decreaseKey(n1, n2) ;
        }
        else {
            steps_sum += h->extractMin();
            count += 1 ;
        }
    }
	std::cout << (double) steps_sum / count << "\t" << size << std::endl ;
}
