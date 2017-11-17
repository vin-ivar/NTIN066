#include <iostream>
#include <fstream>
#include <stdlib.h>
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
    int getLen() ;
};

void node::setParent(node* parent) {
    if(this->parent == parent) return ;
    this->parent = parent ;
    this->next->setParent(parent) ;
}
bool node::isMarked() {
    return this->marked ;
}

int node::getLen() {
    int count = 0 ;
    struct node *n = this ;
    while(true) {
        n = n->next ;
        count += 1 ;
        if(n == this)
            return count ;
    }
}

class Heap {
    struct node* heap ;
    struct node** node_list ;
	// store for array
	int size ;
public:
    Heap(int size) : heap(NULL), size(size) { node_list = new struct node*[size * sizeof(struct node)] ; } ;
   /* ~Heap() {
		for(int i = 0; i < this->size; i++) {
			if(node_list[i])
				delete(node_list[i]) ;
		}
        delete[](node_list) ;
		// cleanup the whole  thing
//		struct node* n = heap ;
//		chain_delete(n) ;
    }
	*/
    void insert(int, int) ;
    struct node* merge(struct node*, struct node*) ;
    int extract_min() ;
    void find_min () ;
    void decreaseKey(int, int) ;
    void _fixParent(struct node*) ;
    void _fixSiblings(struct node*) ;
    void _recurseFix(struct node*) ;
	void chain_delete(struct node*) ;
};

void Heap::insert(int key, int elem) {
    if (!heap) {
        heap = new struct node(key, elem) ;
        node_list[elem * sizeof(struct node*)] = heap ;
        return ;
    }
    struct node* temp = new struct node(key, elem) ;
    node_list[elem * sizeof(struct node*)] = temp ;
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
    struct node* target = node_list[elem * sizeof(struct node*)] ;
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
    this->node_list[min * sizeof(struct node*)] = NULL ;
    if(this->heap->child) {
        this->heap->child->setParent(NULL);
        // count += this->heap->child->getLen() ;
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


    // make an array
    int degree = 0 ;
//    struct node** sizes = new struct node* [32000 * sizeof(struct node)] () ;
    struct node* sizes[16] = {NULL};
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
    /*
    h->insert(95384,0) ;
    h->insert(73932,1) ;
    h->insert(3630,2) ;
    h->insert(67168,3) ;
    h->insert(59304,4) ;
    h->insert(97875,5) ;
    h->insert(32127,6) ;
    h->insert(48682,7) ;
    h->insert(78487,8) ;
    h->insert(30031,9) ;
    h->decreaseKey(8,78480) ;
    h->insert(30527,10) ;
    h->decreaseKey(6,32119) ;
    h->decreaseKey(8,78472) ;
    h->decreaseKey(10,30525) ;
    h->decreaseKey(7,48674) ;
    h->decreaseKey(8,78465) ;
    h->decreaseKey(4,59302) ;
    h->decreaseKey(7,48671) ;
    h->decreaseKey(10,30522) ;
    h->decreaseKey(6,32110) ;
    h->decreaseKey(6,32109) ;
    h->decreaseKey(0,95377) ;
    h->decreaseKey(3,67167) ;
    h->decreaseKey(10,30521) ;
    h->decreaseKey(0,95367) ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
    h->extract_min() ;
    h->decreaseKey(8,78459) ;
    h->decreaseKey(9,30023) ;
    h->decreaseKey(1,73931) ;
    h->decreaseKey(0,95360) ;
    h->decreaseKey(1,73921) ;
    h->insert(15360,11) ;
    h->decreaseKey(2,3621) ;
    delete(h) ;
    */
    std::string line ;
    std::string full_op, remainder ;
    std::string a1, a2 ;
    std::ifstream f ;
    f.open("/home/vinit/Uni/NTIN066/fibheap/temp") ;
    if(!f) {
        std::cout << "fail" ;
    }
    int steps_sum = 0 ;
    int count = 0 ;
    int size = 0 ;
    while(std::getline(std::cin, line)) {
        char op = line[0] ;
        if(op == '#') {
            if(count != 0)
                std::cout << steps_sum / count << "\t" << size << std::endl ;
            delete(h) ;
            size = atoi(line.substr(2, -1).c_str()) ;
            h = new Heap(size) ;
            steps_sum = 0 ;
            count = 0 ;
        }
        else {
            full_op = line.substr(0, 3).c_str() ;
            if(full_op == "INS") {
                remainder = line.substr(4, -1).c_str() ;
                a1 = remainder.substr(0, remainder.find(" ")) ;
                a2 = remainder.substr(remainder.find(" ") + 1, -1) ;
                h->insert(atoi(a2.c_str()), atoi(a1.c_str())) ;
            }
            else if(full_op == "DEC") {
                remainder = line.substr(4, -1).c_str() ;
                a1 = remainder.substr(0, remainder.find(" ")) ;
                a2 = remainder.substr(remainder.find(" ") + 1, -1) ;
                h->decreaseKey(atoi(a1.c_str()), atoi(a2.c_str())) ;
            }
            else {
                steps_sum += h->extract_min();
                count += 1 ;
            }
        }
    }
}
