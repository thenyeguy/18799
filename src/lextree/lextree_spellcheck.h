#ifndef LEXTREE_SPELLCHECK_H
#define LEXTREE_SPELLCHECK_H

#include <stdbool.h>
#include "lextree.h"


/* This spellcheck uses a relative beam pruning. This constant defines the
 * pruning threshold used.
 */
#define LEXTREE_CLOSEST_PRUNING_THRESHOLD 3
 
#define LT_STRING_LENGTH 4096
#define MININT (1<<31)
#define MAXINT (~MININT)


/* Helper node for the lextree_closest code. Contains evalutation state in
 * addition to the topology of the lextree
 */
typedef struct lextree_spellcheck_node lextree_spellcheck_node;
struct lextree_spellcheck_node {
    char c;
    bool is_full_word;

    lextree_spellcheck_node* children[26];
    lextree_spellcheck_node* parent;

    int col;

    int word_length;
    char word[1024];
    int edit_distance;

    int last_word_length;
    char last_word[1024];
    int last_edit_distance;
};


/* lextree_closest - given a lextree and a test string, finds the lowest edit
 *      match to the string in the lextree. If segment is true, then we are
 *      allowed to insert spaces and break the input into multiple words.
 */
char* lextree_closest(lextree* lex, char* string, bool segment);


/* get_lextree_eval_struct - helper for lextree closest. Copies the given
 *      lextree into a lextree_spellcheck structure for evaluation.
 */
lextree_spellcheck_node* get_lextree_eval_struct(lextree_node* l,
    lextree_spellcheck_node* parent);


/* score_lextree_spellcheck_column - given a lextree_spellcheck struct, the test
 *      string, the head of the lextree and whether we allow segmenting into
 *      multiple words, scores one column in the "trellis" of our lextree
 */
void score_lextree_spellcheck_column(lextree_spellcheck_node* n, char* string,
    lextree_spellcheck_node* head, bool segment);

/* prep_lextree_spellcheck_column - swaps the last state and the next state so
 *      we can score the next column
 */
void prep_lextree_spellcheck_column(lextree_spellcheck_node* n);


/* lextree_best_backpointer - given the evaluation structure, returns the lowest
 *      edit distance result in the tree.
 */
lextree_spellcheck_node* lextree_best_backpointer(lextree_spellcheck_node* n);


#endif
