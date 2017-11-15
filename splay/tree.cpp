#include <iostream>
#include <string>
#include <stdlib.h>

struct node {
	public:
	int val ;
	struct node* lc ;
	struct node* rc ;
	struct node* parent ;
	node(int val) : val(val), lc(NULL), rc(NULL), parent(NULL) {} ;
	node(int val, struct node* parent): val(val), lc(NULL), rc(NULL), parent(parent) {} ;
} ;

class Tree {
	bool naive ;
	struct node* root ;
	void RR(struct node*) ;
	void LL(struct node*) ;
	bool isLC(struct node* node) { return node->parent->lc == node ; } ;
	bool isRC(struct node* node) { return node->parent->rc == node ; } ;
	public:
		Tree(std::string str) : root(NULL) { 
			if(str == "--naive") naive = true ;
			else if(str == "--classic") naive = false ;
			else {
				std::cerr << "Usage:\n--naive: naive implementation\n--classic: classic implementation\n" ;
				exit(0) ;
			}
		} ;
		// helpers
		void insert(int val) { 
			if(!root) root = new struct node(val); 
			else insert(val, root) ; 
		}
		int search(int val) { return search(0, val, root) ; } ;
		void display() { display(root) ; } ;

		void insert(int, struct node*) ;
		int search(int, int, struct node*) ;
		void display(struct node*) ;
} ;

void Tree::RR(struct node* node) {
	node->parent->lc = node->rc ;
	if(node->parent->lc)
		node->parent->lc->parent = node->parent ;
	node->rc = node->parent ;
	node->parent = node->rc->parent ;
	if (node->parent) {
		if (node->parent->rc == node->rc)
			node->parent->rc = node ;
		else
			node->parent->lc = node ;
	}
	node->rc->parent = node ;
}

void Tree::LL(struct node* node) {
	node->parent->rc = node->lc ;
	if(node->parent->rc)
		node->parent->rc->parent = node->parent ;
	node->lc = node->parent ;
	node->parent = node->lc->parent ;
	if (node->parent) {
		if (node->parent->lc == node->lc)
			node->parent->lc = node ;
		else
			node->parent->rc = node ;
	}
	node->lc->parent = node ;
}

void Tree::insert(int val, struct node* node) {
	if (val > node->val) {
		if (node->rc) insert(val, node->rc) ;
		else {
			node->rc = new struct node(val, node) ;
			search(0, val, node->rc) ;
		}
	}
	else {
		if (node->lc) insert(val, node->lc) ;
		else {
			node->lc = new struct node(val, node) ;
			search(0, val, node->lc) ;
		}
	}
}

void Tree::display(struct node* node) {
	if(!node) return ;
	std::cout << node->val << std::endl ;
	display(node->lc) ;
	display(node->rc) ;
}

int Tree::search(int len, int val, struct node* node) {
	while(true) {
		if(!node) return len ;
		if(node->val == val) {
			while(true) {
				// break condition
				if(!node->parent) {
					root = node ;
					break ;
				}
				// zig
				if(node->parent == root) {
					if(node == node->parent->lc) RR(node) ;
					else if(node == node->parent->rc) LL(node) ;
				}
				// classical
				else if(!naive) {
					// both are LCs
					if(isLC(node) && isLC(node->parent)) {
						RR(node->parent) ;
						RR(node) ;
					}
					// both are RCs
					else if(isRC(node) && isRC(node->parent)) {
						LL(node->parent) ;
						LL(node) ;
					}
					// x RC p LC
					else if(isRC(node) && isLC(node->parent)) {
						LL(node) ;
						RR(node) ;
					}
					// x LC p RC
					else if(isLC(node) && isRC(node->parent)) {
						RR(node) ;
						LL(node) ;
					}
					
				}
				// naive
				else {
					if (isLC(node)) RR(node) ;
					else if (isRC(node)) LL(node) ;
				}
				
			}
			return len ;
		}
		if(val > node->val) { len += 1 ; node = node->rc ; } ;
		if(val < node->val) { len += 1 ; node = node->lc ; } ;
	}
}

int main(int argc, char *argv[]) {
	if(! argv[1]) {
		std::cerr << "Usage:\n--naive: naive implementation\n--classic: classic implementation\n" ;
		return 0;
	}
	std::string line, arg ;
	arg = argv[1] ;
	
	Tree *t = new Tree(arg) ;
	char op ;
	int num, tot_find, tot_len ;
	while(std::getline(std::cin, line)) {
		char op = line[0] ;
		int num = atoi(line.substr(2, -1).c_str()) ;
		if(op == '#') {
			delete(t) ;
			t = new Tree(arg) ;
			std::cout << tot_len << "\t" << num << "\t" << (double) tot_len / tot_find << std::endl ;
			tot_len = 0 ; tot_find = 0 ;
		}
		else if(op == 'I') t->insert(num) ;
		else if(op == 'F') {
			tot_len += t->search(num) ;
			tot_find += 1 ;
		}
	}	
}
