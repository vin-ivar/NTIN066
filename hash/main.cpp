#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdint.h>

static uint64_t rng_state[2];

static uint64_t rng_rotl(const uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

static uint64_t rng_next_u64(void)
{
	uint64_t s0 = rng_state[0], s1 = rng_state[1];
	uint64_t result = s0 + s1;
	s1 ^= s0;
	rng_state[0] = rng_rotl(s0, 55) ^ s1 ^ (s1 << 14);
	rng_state[1] = rng_rotl(s1, 36);
	return result;
}

static void rng_setup(unsigned int seed)
{
	rng_state[0] = seed * 0xdeadbeef;
	rng_state[1] = seed ^ 0xc0de1234;
}

struct HashTest {
    // actual hash table
    uint64_t *hash_table ;
    // for tabular stuffs
    uint64_t *tabular_1, *tabular_2 ;
    int u, k, c, l ;
    uint64_t U, K, C, L ;
    uint64_t cL ;
    int total_elements ;
    double load_factor ;
    // l -> size of each part of string
    // L -> size of table = no. of parts x 2 ^ size of each part (for indexing)
	uint64_t a_1, a_2 ;
	char *fn, *al ;
public:
    // u -> bits in universe
    // k -> bits in hash table
    // c -> no. of parts to divvy string up into
    // l -> size of each part
    // caps -> 2 ^ lower
    HashTest(int, int, int, char*, char*) ;
    int __rehash() ;
    int hash_fn(uint64_t, int t_no=0) ;
    int __hash_tab(uint64_t, int t_no=0) ;
    int __hash_mul(uint64_t, int t_no=0) ;
    int __hash_mod(uint64_t, int t_no=0) ;
    double insert(uint64_t) ;
    int __insert_coo(uint64_t) ;
    int __insert_lin(uint64_t) ;
    int getSize() {return K ;} ;
    double getLF() {return load_factor ;} ;
};

HashTest::HashTest(int u, int k, int c, char *fn, char *al) : fn(fn), al(al), u(u), k(k), c(c), U(pow(2, u)), K(pow(2, k)), C(pow(2, c)), total_elements(0), load_factor(0) {
    srand(time(NULL)) ;
    l = u / c ;
    L = pow(2, l) ;
    cL = c * pow(2, l) ;

    // init tables and random nums
    // 2 for cuckoo
    hash_table = (uint64_t*) calloc(K, sizeof(uint64_t)) ;
    tabular_1 = (uint64_t*) malloc(cL * sizeof(uint64_t)) ;
    tabular_2 = (uint64_t*) malloc(cL * sizeof(uint64_t)) ;
	do { a_1 = rng_next_u64() % U; } while(a_1 % 2 != 1) ;
	do { a_2 = rng_next_u64() % U; } while(a_2 % 2 != 1) ;

    // table mask = max no. of bits allowed in hash table == k
    int table_mask = K - 1 ;
	for (int i = 0; i < cL; i++) tabular_1[i] = rng_next_u64() & table_mask ;
	for (int i = 0; i < cL; i++) tabular_2[i] = rng_next_u64() & table_mask ;
}

int HashTest::__hash_tab(uint64_t num, int t_no) {
    uint64_t *curr ;
    if(t_no == 0) curr = tabular_1 ;
    else curr = tabular_2 ;

    int mask = 0 ;
    int hash_value = 0 ;
    mask = L - 1 ;
    for(int i = 0 ; i < c; i++) {
        uint64_t temp = i * L + (num & mask) ;
        hash_value ^= curr[temp] ;
        num >>= l ;
    }
    return hash_value ;
}

int HashTest::__hash_mul(uint64_t num, int t_no) {
	uint64_t curr ;
	if(t_no == 0) curr = a_1 ;
	else curr = a_2 ;
    return (unsigned int)((curr * num) % U) / (U / K) ;
}

int HashTest::__hash_mod(uint64_t num, int t_no) {
    return num % K ;
}

int HashTest::__rehash() {
    // easier to initialise stuffs
    int steps = 0 ;
    HashTest h = HashTest(u, k, c, fn, al) ;
    for(int i = 0; i < K; i++) {
        if(hash_table[i] != 0) steps += h.insert(hash_table[i]) ;
    }
    free(this->hash_table) ; free(this->tabular_1) ; free(this->tabular_2) ;
    this->hash_table = h.hash_table ;
    this->tabular_1 = h.tabular_1 ;
    this->tabular_2 = h.tabular_2 ;
    this->a_1 = h.a_1 ; this->a_2 = h.a_2 ;
    return steps ;
}

void swap(uint64_t *a, uint64_t *b) {
    uint64_t temp = *a;
    *a = *b ;
    *b = temp ;
}

int HashTest::__insert_coo(uint64_t num) {
    int cs1 = hash_fn(num, 0) ;
    int cs2 = hash_fn(num, 1) ;
    int pos = hash_fn(num, 0) ;
    if(hash_table[cs1] == num || hash_table[cs2] == num) return 0 ;
    int swaps = 0 ;
    int MAX_TRIES = k * k;
    uint64_t old = num ;
    for(int i = 0; i < MAX_TRIES ; i++) {
        swaps += 1 ;
        if(hash_table[pos] == 0) {
            hash_table[pos] = old ;
            return swaps ;
        }
        swap(&old, &hash_table[pos]) ;
        if(pos == hash_fn(old, 0)) pos = hash_fn(old, 1) ; else pos = hash_fn(old, 0) ;
    }
    swaps += __rehash() ;
    return swaps + __insert_coo(old) ;
}

int HashTest::__insert_lin(uint64_t num) {
    // start with 1 because it will always be at least 1 access
    int accesses = 1 ;
    int table_len = pow(2, k) ;
    int hash_val = hash_fn(num) ;
    while(hash_table[hash_val % table_len] != 0) {
        hash_val++ ;
        if(hash_table[hash_val % table_len] == num) return 0 ;
        accesses++ ;
    }
    hash_table[hash_val % table_len] = num ;
    return accesses ;
}

// switch with appropriate function of choice
int HashTest::hash_fn(uint64_t num, int t_no) {
	if(strcmp(this->fn, "--tab") == 0)
	    return __hash_tab(num, t_no) ;
	else if(strcmp(this->fn, "--mul") == 0)
		return __hash_mul(num, t_no) ;
	else if(strcmp(this->fn, "--mod") == 0)
		return __hash_mod(num, t_no) ;
	else { fprintf(stderr, "Inappropriate function\n") ; exit(0) ; }
}

double HashTest::insert(uint64_t num) {
    clock_t t ;
    if(total_elements == this->K) {
        printf("Hash table full\n") ;
        return -1;
    }
    total_elements++ ;
    load_factor = total_elements / (double) this->K ;
    t = clock() ;
	if(strcmp(this->al, "--lin") ==	0)
	    return __insert_lin(num) ;
	else if(strcmp(this->al, "--coo") == 0)
		return __insert_coo(num) ;
	else {
		fprintf(stderr, "Inappropriate algorithm") ;
		exit(0) ;
	}
    t = clock() - t ;
    return ((double)t * 1000) / CLOCKS_PER_SEC ;
}

int main(int argc, char *argv[]) {
	if(!argv[1] || !argv[2] || !argv[3]) {
		fprintf(stderr, "Usage: [MODE] [FUNCTION] [ALGORITHM]\n" ) ;
		fprintf(stderr, "MODE: -1: part 1 ; -2: part 2\n" ) ;
		fprintf(stderr, "FUNCTION: --tab: tabular hashing ; --mul: multiply-shift ; --mod: modular hashing\n" ) ;
		fprintf(stderr, "ALGORITHM: --lin: linear probing ; --coo: cuckoo hashing\n") ;
		return 0 ;
	}

	if(strcmp(argv[1], "-1") == 0) {
		rng_setup(time(NULL)) ;
		HashTest t(32, 20, 4, argv[2], argv[3]) ;
		int iters = t.getSize() / 100 ;
		int avg_steps ;
		for(int i = 0; i < 100; i++) {
			double avg_lf = 0 ;
			double avg_steps = 0 ;
			for(int j = 0; j < iters; j++) {
				unsigned int x = (unsigned int)rng_next_u64() ;
				avg_steps += t.insert(x) ;
				avg_lf += t.getLF() ;
				// printf("\n---\n") ;
			}
			printf("%f\t%f\n", avg_steps / iters, avg_lf / iters) ;
		}
	}
	else if(strcmp(argv[1], "-2") == 0) {
		rng_setup(time(NULL));
		for (int m = 8; m <= 24; m++) {
			printf("S%d", m) ;
			for (int iter = 0; iter < 10; iter++) {
				int avg = 0;
				int count = 0;
				int avg_steps = 0;
				HashTest t(32, m, 4, argv[2], argv[3]);
				for (int i = 0; i < t.getSize(); i++) {
					avg_steps = t.insert(i + 1) ;
					if (0.91 >= t.getLF() && t.getLF() >= 0.89) {
						avg += avg_steps;
						count += 1 ;
					}
					if (t.getLF() > 0.91) break;
				}
				printf("\t%f", (double)avg / count) ;
			}
			printf("\n");
		}
	}
	else fprintf(stderr, "Bad arguments!\n" ) ;
}
