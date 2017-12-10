#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
/*
    for(int i = 0; i < SIZE ; i++) {
        for(int j = 0; j < SIZE; j++) {
            // printf("%d\t", mat[SIZE * i + j]) ;
        }
        // printf("\n") ;
    }*/
void naiveTrans(int32_t*) ;
void recurseSwap(int, int, int, int, int, int, int, int, int32_t*) ;
void recurseDiag(int, int, int, int, int32_t*) ;

FILE *fp ;
int SIZE ;
double total_time ;
long count ;

int main(int argc, char* argv[]) {
	if(!argv[1]) {
		fprintf(stderr, "Usage:\n-n: naive implementation\n-c: classic implementation\n") ;
		return 0 ;
	}

    fp = fopen("/afs/ms/u/r/ravishav/uni/NTIN066/mat/temp", "w") ;
    srand(time(NULL)) ;
	clock_t t ;
    for(int k = 54; k <= 135; k++) {
		count = 0 ;
		total_time = 0 ;
        SIZE = ceil(pow(2, (double)k / 9)) ;

        int32_t *mat = (int32_t*) malloc(SIZE * SIZE * sizeof(int32_t)) ;
		if(mat == NULL) break ;

		for(int i = 0; i < SIZE * SIZE; i++) {
			mat[i] = (int32_t) rand() ;
		}

		// // printf("%d %d\n", k, SIZE) ;
        // printf("N %d\n", SIZE) ;
		// t = clock() ;
		if(!strcmp(argv[1], "-c")) {
	        recurseDiag(0, SIZE - 1, 0, SIZE - 1, mat) ;
		}
		else if(!strcmp(argv[1], "-n")) {
	        naiveTrans(mat) ;
		}
		// t = clock() - t ;
		printf("%d\t%lf\n", SIZE, total_time) ;
		fprintf(stderr, "%d\t%d\n", k, SIZE) ;
		free(mat) ;
        // printf("E\n") ;
        fflush(stdout) ;
    }
}

void swap(int32_t *a, int32_t *b) {
    int32_t temp = *a ;
    *a = *b ;
    *b = temp ;
}

void index_and_swap(int x1, int y1, int x2, int y2, int32_t *mat) {
	clock_t t ;
	t = clock() ;
    swap(&mat[y1 * SIZE + x1], &mat[y2 * SIZE + x2]) ;
	t = clock() - t ;
	total_time += (double) t / CLOCKS_PER_SEC ;
	count += 1 ;
}

void naiveTrans(int32_t *mat) {
	for(int i = 0; i < SIZE - 1; i++) {
		for(int j = i + 1; j < SIZE; j++) {
            // printf("X %d %d %d %d\n", i, j, j, i) ;
            index_and_swap(i, j, j, i, mat) ;
        }
    }
}

void recurseDiag(int x0, int x1, int y0, int y1, int32_t *mat) {
    // b means bound lol
    int mx, my ;
    mx = std::floor((double)(x1 + x0) / 2) ;
    my = std::floor((double)(y1 + y0) / 2) ;
    // 2 x 2
    if(x1 - x0 == 1 && y1 - y0 == 1) {
        // printf("X %d %d %d %d\n", x1, y0, x0, y1) ;
        index_and_swap(x1, y0, x0, y1, mat) ;
        return ;
    }
    if(x1 - x0 <= 1 || y1 - y0 <= 1) return ;
    recurseDiag(x0, mx, y0, my, mat) ;
    recurseDiag(mx + 1, x1, my + 1, y1, mat) ;
    recurseSwap(mx + 1, x1, y0, my, x0, mx, my + 1, y1, mat) ;

}
void recurseSwap(int Ax0, int Ax1, int Ay0, int Ay1, int Bx0, int Bx1, int By0, int By1, int32_t *mat) {
    int Amx, Amy, Bmx, Bmy ;
    if(Ax1 - Ax0 == 1 && Ay1 - Ay0 == 1) {
        // printf("X %d %d %d %d\n", Ax1, Ay0, Ax0, Ay1) ;
        // printf("X %d %d %d %d\n", Bx1, By0, Bx0, By1) ;
        // printf("X %d %d %d %d\n", Ax0, Ay0, Bx0, By0) ;
        // printf("X %d %d %d %d\n", Ax0, Ay1, Bx0, By1) ;
        // printf("X %d %d %d %d\n", Ax1, Ay0, Bx1, By0) ;
        // printf("X %d %d %d %d\n", Ax1, Ay1, Bx1, By1) ;

        index_and_swap(Ax1, Ay0, Ax0, Ay1, mat) ;
        index_and_swap(Bx1, By0, Bx0, By1, mat) ;
        index_and_swap(Ax0, Ay0, Bx0, By0, mat) ;
        index_and_swap(Ax0, Ay1, Bx0, By1, mat) ;
        index_and_swap(Ax1, Ay0, Bx1, By0, mat) ;
        index_and_swap(Ax1, Ay1, Bx1, By1, mat) ;
        return ;
    }
    if(Ax1 - Ax0 == 0 && Ay1 - Ay0 == 1) {
        // printf("X %d %d %d %d\n", Ax0, Ay0, Bx0, By0) ;
        // printf("X %d %d %d %d\n", Ax0, Ay1, Bx1, By0) ;

        index_and_swap(Ax0, Ay0, Bx0, By0, mat) ;
        index_and_swap(Ax0, Ay1, Bx1, By0, mat) ;
        return ;
    };
    if(Ax1 - Ax0 == 1 && Ay1 - Ay0 == 0) {
        // printf("X %d %d %d %d\n", Ax0, Ay0, Bx0, By0) ;
        // printf("X %d %d %d %d\n", Ax1, Ay0, Bx0, By1) ;

        index_and_swap(Ax0, Ay0, Bx0, By0, mat) ;
        index_and_swap(Ax1, Ay0, Bx0, By1, mat) ;
        return ;
    };
    if(Ax1 - Ax0 == 0 && Ay1 - Ay0 == 0) {
        // printf("X %d %d %d %d\n", Ax0, Ay0, Bx0, By0) ;

        index_and_swap(Ax0, Ay0, Bx0, By0, mat) ;
        return ;
    };
    Amx = std::floor((double)(Ax1 + Ax0) / 2) ;
    Amy = std::floor((double)(Ay1 + Ay0) / 2) ;
    Bmx = std::floor((double)(Bx1 + Bx0) / 2) ;
    Bmy = std::floor((double)(By1 + By0) / 2) ;
    recurseSwap(Ax0, Amx, Ay0, Amy, Bx0, Bmx, By0, Bmy, mat) ;
    recurseSwap(Amx + 1, Ax1, Ay0, Amy, Bx0, Bmx, Bmy + 1, By1, mat) ;
    recurseSwap(Ax0, Amx, Amy + 1, Ay1, Bmx + 1, Bx1, By0, Bmy, mat) ;
    recurseSwap(Amx + 1, Ax1, Amy + 1, Ay1, Bmx + 1, Bx1, Bmy + 1, By1, mat) ;
}
