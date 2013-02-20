#include <stdbool.h>

#define NODE_SIZE (sizeof(trellis_node))
#define MIN(a,b,c) ((c<((a<b)?a:b))?c:((a<b)?a:b))
#define INT_MIN (1<<31)
#define INT_MAX (~INT_MIN)
#define PRINT_TRELLIS false
#define X(x) (x)
#define Y(j,len) (((len)-(j))-1)
#define I(i) (i)
#define J(y,len) (((len)-y)-1)


int ** init_string_trellis(char ** string_array_one,char ** string_array_two);
int ** build_string_trellis(char ** string_array_one,char ** string_array_two);
void populate_string_trellis(int ** trellis, char ** string_array_one,char ** string_array_two);
int get_string_array_length(char ** string_array);


/*
 *	Single Character Trellis
*/


/* Pruning type for the trellis evalutation...
 *     NONE performs no pruning
 *     ABSOLUTE rules out any evaluation that exceeds max edit distance
 *     BEAM rules out any evaluation that exceeds the best score by threshold
 */
typedef enum {
    NONE,
    ABSOLUTE,
    BEAM
} prune_t;
#define TRELLIS_ABSOLUTE_THRESHOLD 3
#define TRELLIS_BEAM_THRESHOLD 3


/* Direction type - for indicating path directions */
typedef enum {
    DIR_NONE,
    DIR_DOWN,
    DIR_LEFT,
    DIR_DOWNLEFT
} trellis_dir;


typedef struct {
	int score;
    bool pruned;
    trellis_dir dir;
} trellis_node;


typedef struct {
    char* word_one;
    int   word_one_len;

    char* word_two;
    int   word_two_len;

    trellis_node** grid;
    int distance;
} search_trellis;


/* build_trellis - given an input word 1 and a corpus word 2 and their lengths,
 *                 computes the final search trellis comparing those words.
 *                 If pruning is not NONE then performs pruning inside the
 *                 search trellis
 */
search_trellis* build_trellis(char* word_one, char* word_two,
                             int word_one_length, int word_two_length,
                             prune_t pruning);


/* alloc_trellis - given the length of input word 1 and corpus word 2,
 *                 allocates the empty trellis structure in memory
 */
search_trellis* alloc_trellis(char* word_one, char* word_two,
                              int word_one_length, int word_two_length);


/* populate_trellis - given an empty trellis structure, the input word 1 and
 *                    the corpus word 2, fills given the trellis structure
 *                    with scores for every node. Performs pruning according
 *                    to the type specified.
 */
void populate_trellis(search_trellis* searchtrellis, prune_t pruning);


/* get_best_score - helper function for populate trellis...
 *                  given the size of the trellis and the node in
 *                  question, stores the best score leading to the
 *                  node given by (i,j) directly
 */
void get_best_score(search_trellis* searchtrellis, int i, int j);


/* print_trellis - given a trellis and its corresponding words, print out the
 *                 scores in table format
 */
void print_trellis(search_trellis* searchtrellis);


/* free_trellis - frees the given trellis structure
 */
void free_trellis(search_trellis* searchtrellis);
