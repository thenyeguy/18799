 #include <stdbool.h>

#define NODE_SIZE (sizeof(trellis_node))
#define MIN(a,b,c) ((c<((a<b)?a:b))?c:((a<b)?a:b))
#define INT_MIN (1<<31)
#define INT_MAX (~INT_MIN)
#define word_print_trellis false

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


/* word_build_trellis - given an input word 1 and a corpus word 2 and their lengths,
 *                 computes the final search trellis comparing those words.
 *                 If pruning is not NONE then performs pruning inside the
 *                 search trellis
 */
trellis_node** word_build_trellis(char* word_one, char* word_two,
                             int word_one_length, int word_two_length,
                             prune_t pruning);


/* word_alloc_trellis - given the length of input word 1 and corpus word 2,
 *                 allocates the empty trellis structure in memory
 */
trellis_node** word_alloc_trellis(int word_one_length, int word_two_length);


/* word_populate_trellis - given an empty trellis structure, the input word 1 and
 *                    the corpus word 2, fills given the trellis structure
 *                    with scores for every node. Performs pruning according
 *                    to the type specified.
 */
void word_populate_trellis(trellis_node** trellis, char* word_one, char* word_two,
                      int word_one_length, int word_two_length,
                      prune_t pruning);


/* word_get_best_score - helper function for populate trellis...
 *                  given the size of the trellis and the node in
 *                  question, stores the best score leading to the
 *                  node given by (i,j) directly
 */
void word_get_best_score(trellis_node** trellis, int i, int j,
                    char* word_one, char* word_two,
                    int word_one_length, int word_two_length);


/* word_print_trellis - given a trellis and its corresponding words, print out the
 *                 scores in table format
 */
 /*
void word_print_trellis(trellis_node** trellis, char* word_one, char* word_two,
                   int word_one_length, int word_two_length);
*/

/* word_free_trellis - frees the given trellis structure
 */
void word_free_trellis(trellis_node** trellis, int word_one_length, int word_two_length);
