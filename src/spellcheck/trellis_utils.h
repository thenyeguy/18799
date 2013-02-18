#include "stdio.h"
#include "strings.h"
#include "stdlib.h"

#define NODE_SIZE (sizeof(trellis_node))
#define Y(length,index) (length-index-1)
#define X(index) (index)
#define MIN(a,b,c) ((c<((a<b)?a:b))?c:((a<b)?a:b))
#define INT_MIN (1<<31)
#define INT_MAX (~INT_MIN)
#define PRINT_TRELLIS 0

typedef struct {
	int score;
}trellis_node;

void print_trellis(trellis_node ** trellis, char * word_one, char * word_two,int word_one_length, int word_two_length);

trellis_node ** build_trellis(char * word_one, char * word_two, int word_one_length, int word_two_length);

void populate_trellis(trellis_node ** trellis, char * word_one, char * word_two,int word_one_length, int word_two_length);

trellis_node ** initialize_trellis(int word_one_length, int word_two_length);

int get_next_best_score(trellis_node ** trellis,int i,int j,char * word_one, char * word_two,int word_one_length,int word_two_length);

void free_trellis(trellis_node ** trellis,int word_one_length,int word_two_length);


